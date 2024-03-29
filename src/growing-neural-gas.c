/*
   growing-neural-gas.c --- Simple library for "growing neural gas with utility" algorithm
   "https://ru.wikipedia.org/wiki/Нейронный_газ"
   "https://en.wikipedia.org/wiki/Neural_gas"

   based on main article:
   "http://www.ks.uiuc.edu/Publications/Papers/PDF/MART91B/MART91B.pdf"
   Thomas Martinetz and Klaus Schulten (1991).
   "A "neural gas" network learns topologies".
   Artificial Neural Networks. Elsevier. pp. 397–402.

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>

   This file is part of c-artificial-neural-network.

   c-artificial-neural-network is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   c-artificial-neural-network is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with c-artificial-neural-network.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <math.h>
#include <float.h> // #include <limits.h>
#include <stdlib.h> // for qsort

#include "growing-neural-gas.h"
#include "vector.h"


void initialization (float eps_winner,
		     float eps_neighbour,
		     float eps_local_error,
		     float factor_beta_decrease_local_error,
		     float limit_conn_age,
		     float k_utility,
		     unsigned long int lambda_step,
		     int limit_network_size,
		     int dimension_of_sensor,
		     GNG_NEURON *gng)
{
	gng->eps_winner = eps_winner;
	gng->eps_neighbour = eps_neighbour;
	gng->eps_local_error = eps_local_error;
	gng->factor_beta_decrease_local_error = factor_beta_decrease_local_error;
	gng->limit_conn_age = limit_conn_age;
	gng->k_utility = k_utility;
	gng->lambda_step = lambda_step;
	gng->limit_network_size = limit_network_size;
	gng->dimension_of_sensor = dimension_of_sensor;

	for (int i = 0; i < gng->limit_network_size; i++) {
		gng[i].active = GNG_OFF;
		/* fixme: memset for float? */
		for (int j = 0; j < gng->dimension_of_sensor; j++) gng[i].weight[j] = 0.0;
		for (int j = 0; j < gng->limit_network_size; j++) gng[i].conn_age[j] = GNG_NOT_CONNECTED;
		gng[i].local_error = 0.0;
		gng[i].utility_factor = 0.0;
		gng[i].group = GNG_NOT_IN_ANY_GROUPS;
	}
}



int print_gng_neuron (int dimension_of_sensor, int limit_network_size, GNG_NEURON neuron)
{
	if (neuron.active == GNG_OFF) {
		fprintf(stderr, "-\n");
		return 0;
	}

	fprintf(stderr, "w:");
	for (int i = 0; i < dimension_of_sensor; i++) {
		fprintf(stderr, " %7.1f", neuron.weight[i]);
	}

	fprintf(stderr, "\ta:");
	for (int i = 0; i < limit_network_size; i++) {
		if (neuron.conn_age[i] < GNG_INITIAL_CONNECTION_AGE)
			printf (" -");
		else
			fprintf(stderr, " %d", neuron.conn_age[i]);
	}

	fprintf(stderr, "\te: %5.3f\t", neuron.local_error);
	fprintf(stderr, "u: %5.3f\t", neuron.utility_factor);
	fprintf(stderr, "g: %d\n", neuron.group);

	return 0;
}



// fixme: print-gng-as-list (gng)



int add_gng_neuron (GNG_NEURON *gng)
{
	int index = -1;
	int i = 0;
	while ((gng[i].active == GNG_ON) && i < gng->limit_network_size) {i++;}
	if (i < gng->limit_network_size) {
		gng[i].active = GNG_ON;
		for (int j = 0; j < gng->dimension_of_sensor; j++) gng[i].weight[j] = 0.0;
		for (int j = 0; j < gng->limit_network_size; j++) gng[i].conn_age[j] = GNG_NOT_CONNECTED;
		gng[i].local_error = 0.0;
		gng[i].utility_factor = 0.0;
		gng[i].group = GNG_NOT_IN_ANY_GROUPS;

		index = i;
	}
	return index;
}



void find_and_del_neuron_with_min_utility_factor (GNG_NEURON *gng)
{
	float E_median = value_of_median_local_error (gng);
	int index_of_U_min = index_of_minimum_utility_factor (gng);

	if (length_gng (gng) > 2    &&
	    (E_median / gng[index_of_U_min].utility_factor) > gng->k_utility) {
		fprintf(stderr, "[d]Emedian=%f Umin[%d]=%f    Emedian/Umin=%f > k=%f\n", E_median, index_of_U_min, gng[index_of_U_min].utility_factor, E_median / gng[index_of_U_min].utility_factor, gng->k_utility);
		/* delete-neuron-U-min */
		gng[index_of_U_min].active = GNG_OFF;

		/* make-consistent-gng */
		/* remove-unexisted-conn-age */
		for (int i = 0; i < gng->limit_network_size; i++) {
			if (gng[i].active == GNG_ON) {
				gng[i].conn_age[index_of_U_min] = GNG_NOT_CONNECTED;
			}
		}

		reconnect (gng); // fixme: really need?
	}
}


/* search Umin and Umax for node diameter */
// fixme: many duplicated functions: extremum_utility_factor, index_of_minimum_utility_factor
void extremum_utility_factor (float *Umin, float *Umax, GNG_NEURON *gng)
{
	int counter = 0;
	*Umin = *Umax = 0.0; // fix warning
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON) {
			if (counter == 0) { /* choise first element */
				counter ++;
				*Umin = *Umax = gng[i].utility_factor;
			} else { /* find extremum */
				if (*Umin > gng[i].utility_factor) {*Umin = gng[i].utility_factor;}
				if (*Umax < gng[i].utility_factor) {*Umax = gng[i].utility_factor;}
			}
		}
	}
}


int index_of_minimum_utility_factor (GNG_NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = 0;
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON) {
			if (counter == 0) { /* choise first element */
				counter ++;
				index = i;
				value = gng[i].utility_factor;
			} else { /* find extremum */
				if (value > gng[i].utility_factor) {
					index = i;
					value = gng[i].utility_factor;
				}
			}
		}
	}
	return index;
}



int length_gng (GNG_NEURON *gng)
{
	int len = 0;
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON) len++;
	}
	return len;
}



static int cmp_neuron_local_error_with_index(const void * a, const void * b)
{ // fixme: remove function?
	GNG_LOCALERROR *leA = (GNG_LOCALERROR *)a;
	GNG_LOCALERROR *leB = (GNG_LOCALERROR *)b;

	return leA->local_error > leB->local_error ? 1 : -1;
}



// fixme: it is necessary to combine the calculations for finding the values of "maximum local error" and "median local error"
int index_of_median_local_error (GNG_NEURON *gng)
{ // fixme: remove function?
	GNG_LOCALERROR lerr_arr[gng->limit_network_size];
	int counter = 0;

	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON ) {
			lerr_arr[counter].local_error = gng[i].local_error;
			lerr_arr[counter].index_in_gng = i;
			counter++;
		}
	}

	qsort(lerr_arr, counter, sizeof(GNG_LOCALERROR *), cmp_neuron_local_error_with_index); /* for thread use qsort_r */

	return lerr_arr[counter/2].index_in_gng;
}




static int cmp_neuron_local_error(const void * a, const void * b)
{
	return *(float*)a > *(float*)b ? 1 : -1;
}


// fixme: it is necessary to combine the calculations for finding the values of "maximum local error" and "median local error"
float value_of_median_local_error (GNG_NEURON *gng)
{
	float lerr_arr[gng->limit_network_size];
	int counter = 0;

	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON ) {
			lerr_arr[counter] = gng[i].local_error;
			counter++;
		}
	}

	qsort(lerr_arr, counter, sizeof(float), cmp_neuron_local_error); /* for thread use qsort_r */

	return lerr_arr[counter/2];
}



/*
  if set very aggressive coefficients, then network delete all
  neurons and leave two (maybe unconnected) so need reconnect them
*/
void reconnect (GNG_NEURON *gng)
{
	int len = length_gng (gng);
	int neuron_a = -1;
	int neuron_b = -1;

	if (len <= 2) {
		for (int i = 0; i < gng->limit_network_size; i++) {
			if (gng[i].active == GNG_ON ) {
				if (neuron_a == -1) {
					neuron_a = i;
				} else {
					if (neuron_b == -1) {
						neuron_b = i;
						set_neuron_conn_age (neuron_a, neuron_b, GNG_INITIAL_CONNECTION_AGE, gng);
						break;
					}
				}
			}
		}
	}

}



void update_neuron_weight_vector (int neuron_a, float step, float *sensor, GNG_NEURON *gng)
{
	for (int i = 0; i < gng->dimension_of_sensor; i++) {
		gng[neuron_a].weight[i] += step*(sensor[i] - gng[neuron_a].weight[i]);
	}
}



void update_neighbours_weights (int neuron_a, float eps_step, float *sensor, GNG_NEURON *gng)
{
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[neuron_a].conn_age[i] >= GNG_INITIAL_CONNECTION_AGE) {
			update_neuron_weight_vector (i, eps_step, sensor, gng);
		}
	}
}



void update_neuron_conn_age (int neuron_a, int neuron_b, int step, GNG_NEURON *gng)
{
	if (gng[neuron_a].conn_age[neuron_b] == GNG_NOT_CONNECTED) {
		// fixme: make algorithm slightly slow, but use only for visual enchanced
		ungroup (neuron_a, gng);
		ungroup (neuron_b, gng);
	}
	gng[neuron_a].conn_age[neuron_b] += step;
	gng[neuron_b].conn_age[neuron_a] += step;
}



void set_neuron_conn_age (int neuron_a, int neuron_b, int conn_age, GNG_NEURON *gng)
{
	if (gng[neuron_a].conn_age[neuron_b] == GNG_NOT_CONNECTED || conn_age == GNG_NOT_CONNECTED) {
		// fixme: make algorithm slightly slow, but use only for visual enchanced
		ungroup (neuron_a, gng);
		ungroup (neuron_b, gng);
	}
	gng[neuron_a].conn_age[neuron_b] = conn_age;
	gng[neuron_b].conn_age[neuron_a] = conn_age;
}


/*
  increase by 1 neighbours connection age
*/
void inc_neighbours_conn_age (int neuron_a, GNG_NEURON *gng)
{
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[neuron_a].conn_age[i] >= GNG_INITIAL_CONNECTION_AGE) {
			update_neuron_conn_age (neuron_a, i, 1, gng);
		}
	}
}



void remove_old_conn_age (GNG_NEURON *gng)
{
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON) {
			for (int j = i; j < gng->limit_network_size; j++) {
				if (gng[i].conn_age[j] > gng->limit_conn_age)
					set_neuron_conn_age (i, j, GNG_NOT_CONNECTED, gng);
			}
		}
	}

}



void update_neuron_local_error (int neuron_a, float step, GNG_NEURON *gng)
{
	gng[neuron_a].local_error += step;
}



void update_neuron_utility_factor (int neuron_a, float step, GNG_NEURON *gng)
{
	gng[neuron_a].utility_factor += step;
}



void decrease_all_neuron_local_errors_and_utility_factor (GNG_NEURON *gng)
{
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON ) {
			gng[i].local_error *= 1.0 - gng->factor_beta_decrease_local_error;
			gng[i].utility_factor *= 1.0 - gng->factor_beta_decrease_local_error;
		}
	}
}



void calculate_distance_weight_sensor (float *sensor, GNG_NEURON *gng, float *return_distance)
{
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON ) {
			return_distance[i] = euclidean_distance_vector (gng[i].weight, sensor, gng->dimension_of_sensor);
		} else {
			return_distance[i] = FLT_MAX;
		}
	}
}



/*
  because: euclidean-distance not good for cyclic data (angles, ...)
  functions-mixed-space == (list euclidean-distance euclidean-distance angle-distance angle-distance ...)
  dimension of functions-mixed-space --- equivalent to dimension of sensor
*/
void calculate_distance_in_mixed_space_weight_sensor (int *mixed_space, float *sensor, GNG_NEURON *gng, float *return_distance)
{
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON ) {
			float sum = 0.0;
			for (int j = 0; j < gng->dimension_of_sensor; j++) {
				switch (mixed_space[j]) {
				case GNG_ANGLE:
					sum += angle_distance (gng[i].weight[j], sensor[j]);
					break;
				case GNG_EUCLIDEAN:
				default:
					sum += euclidean_distance (gng[i].weight[j], sensor[j]);
					break;
				}
			}
			return_distance[i] = sqrt(sum);
		} else {
			return_distance[i] = FLT_MAX;
		}
	}
}



void find_index_of_two_minimal (float *in_arr, int in_size, int *out_indexes)
{
	int index_a, index_b;
	float val_a, val_b;

	if (in_arr[0] <= in_arr[1]) {
		index_a = 0;
		val_a = in_arr[0];
		index_b = 1;
		val_b = in_arr[1];
	} else {
		index_a = 1;
		val_a = in_arr[1];
		index_b = 0;
		val_b = in_arr[0];
	}

	for (int i = 2; i < in_size; i++) {
		if (in_arr[i] < val_a) {
			index_b = index_a;
			val_b = val_a;
			index_a = i;
			val_a = in_arr[i];
		} else {
			if (in_arr[i] < val_b && i != index_a) {
				index_b = i;
				val_b = in_arr[i];
			}
		}
	}

	out_indexes[0] = index_a;
	out_indexes[1] = index_b;
}



// fixme: it is necessary to combine the calculations for finding the values of "maximum local error" and "median local error"
int find_neuron_index_with_max_local_error (GNG_NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = 0;
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON) {
			if (counter == 0) { /* choise first element */
				counter ++;
				index = i;
				value = gng[i].local_error;
			} else { /* find extremum */
				if (value < gng[i].local_error) {
					index = i;
					value = gng[i].local_error;
				}
			}
		}
	}
	return index;
}



// fixme: it is necessary to combine the calculations for finding the values of "maximum local error" and "median local error"
/* if not found, then return index_max_local_error */
int find_neighbours_index_with_max_local_error (int index_max_local_error, GNG_NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = -1;
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[index_max_local_error].conn_age[i] >= GNG_INITIAL_CONNECTION_AGE) {
			if (counter == 0) { /* choise first element */
				counter ++;
				index = i;
				value = gng[i].local_error;
			} else { /* find extremum */
				if (value < gng[i].local_error) {
					index = i;
					value = gng[i].local_error;
				}
			}
		}
	}
	return index;
}



void adaptive_step_create_new_neuron (GNG_NEURON *gng)
{
	int index_neuron_max_local_error = find_neuron_index_with_max_local_error (gng); // algorithm:13
	int index_neighbour_for_max_local_error = find_neighbours_index_with_max_local_error (index_neuron_max_local_error, gng); // algorithm:14
	int index_of_new_neuron = add_gng_neuron(gng); // algorithm:15.a

	if (index_neighbour_for_max_local_error < 0) { // fix situation when all index = -1
		// more correct solution: use not aggressive coefficients (k_utility)
		for (int i = 0; i < gng->limit_network_size; i++) {
			if ((gng[i].active == GNG_ON) &&
			    index_neighbour_for_max_local_error != index_neuron_max_local_error) { index_neighbour_for_max_local_error = i; }
		}
	}

	// fixme: move "algorithm:15.b" to function update_neuron_weight_vector
	// algorithm:15.b
	for (int i = 0; i < gng->dimension_of_sensor; i++) {
		gng[index_of_new_neuron].weight[i] = (gng[index_neuron_max_local_error].weight[i] +
						      gng[index_neighbour_for_max_local_error].weight[i]) / 2.0;
	}



	// algorithm:16
	set_neuron_conn_age(index_neuron_max_local_error,
			    index_of_new_neuron,
			    gng[index_neuron_max_local_error].conn_age[index_neighbour_for_max_local_error],
			    gng);
	set_neuron_conn_age(index_of_new_neuron,
			    index_neighbour_for_max_local_error,
			    gng[index_neuron_max_local_error].conn_age[index_neighbour_for_max_local_error],
			    gng);
	set_neuron_conn_age(index_neuron_max_local_error,
			    index_neighbour_for_max_local_error,
			    GNG_NOT_CONNECTED,
			    gng);



	// algorithm:17
	gng[index_of_new_neuron].utility_factor = (gng[index_neuron_max_local_error].utility_factor +
						   gng[index_neighbour_for_max_local_error].utility_factor) / 2.0;

	// algorithm:18
	gng[index_neuron_max_local_error].local_error *= gng->eps_local_error;
	gng[index_neighbour_for_max_local_error].local_error *= gng->eps_local_error;
	gng[index_of_new_neuron].local_error = gng[index_neuron_max_local_error].local_error;
}



void growing_neural_gas (unsigned long int epoch, int winners[2], int *mixed_space, float *sensor, GNG_NEURON *gng)
{
	float distances_w_s[gng->limit_network_size];
	if (mixed_space == NULL) {
		calculate_distance_weight_sensor (sensor, gng, distances_w_s);
	} else {
		calculate_distance_in_mixed_space_weight_sensor (mixed_space, sensor, gng, distances_w_s);
	}

	// algorithm:04
	find_index_of_two_minimal (distances_w_s, gng->limit_network_size, winners);

	// algorithm:05
	update_neuron_local_error (winners[0], square (distances_w_s[winners[0]]), gng);

	// algorithm:07 for winner
	update_neuron_weight_vector (winners[0], gng->eps_winner, sensor, gng);

	// algorithm:07 for neighbours
	update_neighbours_weights (winners[0], gng->eps_neighbour, sensor, gng);

	// algorithm:08
	update_neuron_utility_factor (winners[0], square (distances_w_s[winners[1]]) - square (distances_w_s[winners[0]]), gng);

	// algorithm:09
	inc_neighbours_conn_age (winners[0], gng);

	// algorithm:10: set connection to 0 (*initial-connection-age*) between two winners
	set_neuron_conn_age (winners[0], winners[1], GNG_INITIAL_CONNECTION_AGE, gng);

	// algorithm:11.a
	remove_old_conn_age (gng);

	// algorithm:11.b
	find_and_del_neuron_with_min_utility_factor (gng);

	// algorithm:12
	if ((epoch % gng->lambda_step == 0) &&
	    (gng->limit_network_size > length_gng (gng))) {
		adaptive_step_create_new_neuron (gng);
	}

	// algorithm:20,21
	decrease_all_neuron_local_errors_and_utility_factor (gng);
}


/* return group number if exist */
// fixme: need more unit-testings
int recursive_search_group (int neuron_a, GNG_NEURON *gng)
{
	int group_number = GNG_NOT_IN_ANY_GROUPS;
	
	if (gng[neuron_a].active == GNG_ON && gng[neuron_a].group == GNG_NOT_IN_ANY_GROUPS) {
		gng[neuron_a].group = GNG_SEARCH_GROUPS;
	}
	
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[neuron_a].conn_age[i] >= GNG_INITIAL_CONNECTION_AGE &&
		    gng[i].active == GNG_ON) {
			if (gng[i].group == GNG_NOT_IN_ANY_GROUPS) {
				group_number = recursive_search_group (i, gng);
			} else {
				if (gng[i].group != GNG_SEARCH_GROUPS) {
					// found it!
					return gng[i].group;
				}
			}
		}
	}

	return group_number;
}

void recursive_set_group (int neuron_a, int group_number, GNG_NEURON *gng)
{
	gng[neuron_a].group = group_number;
		
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[neuron_a].conn_age[i] >= GNG_INITIAL_CONNECTION_AGE &&
		    gng[i].active == GNG_ON &&
		    (gng[i].group == GNG_NOT_IN_ANY_GROUPS || gng[i].group == GNG_SEARCH_GROUPS || gng[i].group != group_number)) {
			recursive_set_group (i, group_number, gng);
		}
	}
}



void extract_groups_from_conn_ages (GNG_NEURON *gng)
{
	int group_number;
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON && gng[i].group == GNG_NOT_IN_ANY_GROUPS) {
			/* first run for find existed group */
			group_number = recursive_search_group(i, gng);
			/* second run for assign group */
			if (group_number == GNG_NOT_IN_ANY_GROUPS || group_number == GNG_SEARCH_GROUPS) {recursive_set_group(i, i, gng);}
			else {recursive_set_group(i, group_number, gng);}
		}
	}
}



void ungroup (int neuron_a, GNG_NEURON *gng)
{
	// fixme: if group splitted to two (or more) groups, one of group (bigest?, most heavy?) can continue use old group_number
	int group_for_del = gng[neuron_a].group;
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON && gng[i].group == group_for_del) {
			gng[i].group = GNG_NOT_IN_ANY_GROUPS;
		}
	}
}



/* return 0 if all good */
int write_gng_to_file (char *file_name, GNG_NEURON *gng)
{
	FILE * ifp;
	size_t result;
	int sum_res = 0;

	ifp = fopen(file_name, "wb");
	if (ifp != NULL) {
		for (int i = 0; i < gng->limit_network_size; i++) {
			/* fixme: not need to write adress of weight
			 * and conn_age (because it is useless
			 * information for next run of programm) */
			result = fwrite(&(gng[i]), sizeof(struct gng_Neuron), 1, ifp);

			sum_res += (result == 1) ? 1 : 0;
			result = fwrite(gng[i].weight, gng->dimension_of_sensor * sizeof (* (gng[i].weight)), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
			result = fwrite(gng[i].conn_age, gng->limit_network_size * sizeof (* (gng[i].conn_age)), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
		}
		fclose(ifp);
	}
	return (sum_res == 3 * gng->limit_network_size) ? 0 : 1; /* 3 = gng + weight + conn_age */
}


/* return 0 if all good */
int read_gng_from_file (char *file_name, GNG_NEURON *gng)
{
	FILE * ifp;
	size_t result;
	int sum_res = 0;

	// fixme: for restore adress of weight and conn_age
	float *weight;
	int *conn_age;

	ifp = fopen(file_name, "rb");
	if (ifp != NULL) {
		for (int i = 0; i < gng->limit_network_size; i++) {
			weight = gng[i].weight;// fixme: store adress of weight
			conn_age = gng[i].conn_age;// fixme: store adress of conn_age
			result = fread(&(gng[i]), sizeof(GNG_NEURON), 1, ifp);
			gng[i].weight = weight;// fixme: restore adress of weight
			gng[i].conn_age = conn_age;// fixme: restore adress of conn_age

			sum_res += (result == 1) ? 1 : 0;
			result = fread(gng[i].weight, sizeof (* (gng[i].weight)), gng->dimension_of_sensor, ifp);
			sum_res += (result == 1) ? 1 : 0;
			result = fread(gng[i].conn_age, sizeof (* (gng[i].conn_age)), gng->limit_network_size, ifp);
			sum_res += (result == 1) ? 1 : 0;
		}
		fclose(ifp);
	}
	return (sum_res == 3 * gng->limit_network_size) ? 0 : 1; /* 3 = gng + weight + conn_age */
}
