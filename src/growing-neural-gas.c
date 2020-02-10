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


void initialization (int dimension_of_sensor, int limit_network_size, NEURON *gng)
{
	for (int i=0; i<limit_network_size; i++) {
		gng[i].active = OFF;
		/* fixme: memset for float? */
		for (int j=0; j<dimension_of_sensor; j++) gng[i].weight[j] = 0.0;
		for (int j=0; j<limit_network_size; j++) gng[i].conn_age[j] = NOT_CONNECTED;
		gng[i].local_error = 0.0;
		gng[i].utility_factor = 0.0;
		gng[i].group = NOT_IN_ANY_GROUPS;
	}
}



int print_neuron (int dimension_of_sensor, int limit_network_size, NEURON neuron)
{
	if (neuron.active == OFF) {
		printf("-\n");
		return 0;
	}

	printf("w:");
	for (int i=0; i<dimension_of_sensor; i++) {
		printf(" %7.1f", neuron.weight[i]);
	}

	printf("\ta:");
	for (int i=0; i<limit_network_size; i++) {
		if (neuron.conn_age[i] < INITIAL_CONNECTION_AGE)
			printf (" -");
		else
			printf(" %d", neuron.conn_age[i]);
	}

	printf("\te: %5.3f\t", neuron.local_error);
	printf("u: %5.3f\t", neuron.utility_factor);
	printf("g: %d\n", neuron.group);

	return 0;
}



// fixme: print-gng-as-list (gng)



int add_neuron (int dimension_of_sensor, int limit_network_size, NEURON *gng)
{
	int index = -1;
	int i = 0;
	while ((gng[i].active == ON) && i < limit_network_size) {i++;}
	if (i < limit_network_size) {
		gng[i].active = ON;
		for (int j=0; j<dimension_of_sensor; j++) gng[i].weight[j] = 0.0;
		for (int j=0; j<limit_network_size; j++) gng[i].conn_age[j] = NOT_CONNECTED;
		gng[i].local_error = 0.0;
		gng[i].utility_factor = 0.0;
		gng[i].group = NOT_IN_ANY_GROUPS;

		index = i;
	}
	return index;
}



void find_and_del_neuron_with_min_utility_factor (float k, int limit_network_size, NEURON *gng)
{
	float E_median = value_of_median_local_error (limit_network_size, gng);
	int index_of_U_min = index_of_minimum_utility_factor (limit_network_size, gng);

	if (length_gng (limit_network_size, gng) > 2    &&
	    (E_median / gng[index_of_U_min].utility_factor) > k) {
		printf("[d]Emedian=%f Umin[%d]=%f    Emedian/Umin=%f > k=%f\n", E_median, index_of_U_min, gng[index_of_U_min].utility_factor, E_median / gng[index_of_U_min].utility_factor, k);
		/* delete-neuron-U-min */
		printf("remove neuron number %d\n", index_of_U_min);
		gng[index_of_U_min].active = OFF;

		/* make-consistent-gng */
		/* remove-unexisted-conn-age */
		for (int i=0; i<limit_network_size; i++) {
			if (gng[i].active == ON) {
				gng[i].conn_age[index_of_U_min] = NOT_CONNECTED;
			}
		}

		reconnect (limit_network_size, gng); // fixme: really need?
	}
}



int index_of_minimum_utility_factor (int limit_network_size, NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = 0;
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON) {
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



int length_gng (int limit_network_size, NEURON *gng)
{
	int len = 0;
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON) len++;
	}
	return len;
}



static int cmp_neuron_local_error_with_index(const void * a, const void * b)
{ // fixme: remove function?
	LOCALERROR *leA = (LOCALERROR *)a;
	LOCALERROR *leB = (LOCALERROR *)b;

	return leA->local_error > leB->local_error ? 1 : -1;
}



// fixme: it is necessary to combine the calculations for finding the values of "maximum local error" and "median local error"
int index_of_median_local_error (int limit_network_size, NEURON *gng)
{ // fixme: remove function?
	LOCALERROR lerr_arr[limit_network_size];
	int counter = 0;

	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON ) {
			lerr_arr[counter].local_error = gng[i].local_error;
			lerr_arr[counter].index_in_gng = i;
			counter++;
		}
	}

	qsort(lerr_arr, counter, sizeof(LOCALERROR *), cmp_neuron_local_error_with_index); /* for thread use qsort_r */

	return lerr_arr[counter/2].index_in_gng;
}




static int cmp_neuron_local_error(const void * a, const void * b)
{
	return *(float*)a > *(float*)b ? 1 : -1;
}


// fixme: it is necessary to combine the calculations for finding the values of "maximum local error" and "median local error"
float value_of_median_local_error (int limit_network_size, NEURON *gng)
{
	float lerr_arr[limit_network_size];
	int counter = 0;

	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON ) {
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
void reconnect (int limit_network_size, NEURON *gng)
{
	int len = length_gng (limit_network_size, gng);
	int neuron_a = -1;
	int neuron_b = -1;

	if (len <= 2) {
		for (int i=0; i<limit_network_size; i++) {
			if (gng[i].active == ON ) {
				if (neuron_a == -1) {
					neuron_a = i;
				} else {
					if (neuron_b == -1) {
						neuron_b = i;
						set_neuron_conn_age (neuron_a, neuron_b, INITIAL_CONNECTION_AGE, gng);
						break;
					}
				}
			}
		}
	}

}



void update_neuron_weight_vector (int neuron_a, float step, float *sensor, int dimension_of_sensor, NEURON *gng)
{
	for (int i=0; i<dimension_of_sensor; i++) {
		gng[neuron_a].weight[i] += step*(sensor[i] - gng[neuron_a].weight[i]);
	}
}



void update_neighbours_weights (int neuron_a, float eps_step, float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[neuron_a].conn_age[i] >= INITIAL_CONNECTION_AGE) {
			update_neuron_weight_vector (i, eps_step, sensor, dimension_of_sensor, gng);
		}
	}
}



void update_neuron_conn_age (int neuron_a, int neuron_b, int step, NEURON *gng)
{
	gng[neuron_a].conn_age[neuron_b] += step;
	gng[neuron_b].conn_age[neuron_a] += step;
}



void set_neuron_conn_age (int neuron_a, int neuron_b, int conn_age, NEURON *gng)
{
	gng[neuron_a].conn_age[neuron_b] = conn_age;
	gng[neuron_b].conn_age[neuron_a] = conn_age;
}


/*
  increase by 1 neighbours connection age
*/
void inc_neighbours_conn_age (int neuron_a, int limit_network_size, NEURON *gng)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[neuron_a].conn_age[i] >= INITIAL_CONNECTION_AGE) {
			update_neuron_conn_age (neuron_a, i, 1, gng);
		}
	}
}



void remove_old_conn_age (int limit_conn_age, int limit_network_size, NEURON *gng)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON) {
			for (int j=i; j<limit_network_size; j++) {
				if (gng[i].conn_age[j] > limit_conn_age)
					set_neuron_conn_age (i, j, NOT_CONNECTED, gng);
			}
		}
	}

}



void update_neuron_local_error (int neuron_a, float step, NEURON *gng)
{
	gng[neuron_a].local_error += step;
}



void update_neuron_utility_factor (int neuron_a, float step, NEURON *gng)
{
	gng[neuron_a].utility_factor += step;
}



void decrease_all_neuron_local_errors_and_utility_factor (float factor_beta, int limit_network_size, NEURON *gng)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON ) {
			gng[i].local_error *= 1.0 - factor_beta;
			gng[i].utility_factor *= 1.0 - factor_beta;
		}
	}
}



void calculate_distance_weight_sensor (float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng, float *return_distance)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON ) {
			return_distance[i] = euclidean_distance_vector (gng[i].weight, sensor, dimension_of_sensor);
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
void calculate_distance_in_mixed_space_weight_sensor (int *mixed_space, float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng, float *return_distance)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON ) {
			float sum = 0.0;
			for (int j=0; j<dimension_of_sensor; j++) {
				switch (mixed_space[j]) {
				case ANGLE:
					sum += angle_distance (gng[i].weight[j], sensor[j]);
					break;
				case EUCLIDEAN:
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

	for (int i=2; i<in_size; i++) {
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
int find_neuron_index_with_max_local_error (int limit_network_size, NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = 0;
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON) {
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
int find_neighbours_index_with_max_local_error (int index_max_local_error, int limit_network_size, NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = -1;
	for (int i=0; i<limit_network_size; i++) {
		if (gng[index_max_local_error].conn_age[i] >= INITIAL_CONNECTION_AGE) {
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



void adaptive_step_create_new_neuron (float eps_local_error, int dimension_of_sensor, int limit_network_size, NEURON *gng)
{
	int index_neuron_max_local_error = find_neuron_index_with_max_local_error (limit_network_size, gng); // algorithm:13
	int index_neighbour_for_max_local_error = find_neighbours_index_with_max_local_error (index_neuron_max_local_error, limit_network_size, gng); // algorithm:14
	int index_of_new_neuron = add_neuron(dimension_of_sensor, limit_network_size, gng); // algorithm:15.a

	if (index_neighbour_for_max_local_error < 0) { // fix situation when all index = -1
		// more correct solution: use not aggressive coefficients (k_utility)
		for (int i=0; i<limit_network_size; i++) {
			if ((gng[i].active == ON) &&
			    index_neighbour_for_max_local_error != index_neuron_max_local_error) { index_neighbour_for_max_local_error = i; }
		}
	}

	// fixme: move "algorithm:15.b" to function update_neuron_weight_vector
	// algorithm:15.b
	for (int i=0; i<dimension_of_sensor; i++) {
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
			    NOT_CONNECTED,
			    gng);



	// algorithm:17
	gng[index_of_new_neuron].utility_factor = (gng[index_neuron_max_local_error].utility_factor +
						   gng[index_neighbour_for_max_local_error].utility_factor) / 2.0;

	// algorithm:18
	gng[index_neuron_max_local_error].local_error *= eps_local_error;
	gng[index_neighbour_for_max_local_error].local_error *= eps_local_error;
	gng[index_of_new_neuron].local_error = gng[index_neuron_max_local_error].local_error;
}



void growing_neural_gas (int epoch, float eps_winner, float eps_neighbour, float eps_local_error, float factor_beta_decrease_local_error, int limit_conn_age, float k_utility, int lambda_step, int *mixed_space, float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng)
{
	float distances_w_s[limit_network_size];
	if (mixed_space == NULL) {
		calculate_distance_weight_sensor (sensor, dimension_of_sensor, limit_network_size, gng, distances_w_s);
	} else {
		calculate_distance_in_mixed_space_weight_sensor (mixed_space, sensor, dimension_of_sensor, limit_network_size, gng, distances_w_s);
	}

	// algorithm:04
	int winners[2];
	find_index_of_two_minimal (distances_w_s, limit_network_size, winners);

	// algorithm:05
	update_neuron_local_error (winners[0], square (distances_w_s[winners[0]]), gng);

	// algorithm:07 for winner
	update_neuron_weight_vector (winners[0], eps_winner, sensor, dimension_of_sensor, gng);

	// algorithm:07 for neighbours
	update_neighbours_weights (winners[0], eps_neighbour, sensor, dimension_of_sensor, limit_network_size, gng);

	// algorithm:08
	update_neuron_utility_factor (winners[0], square (distances_w_s[winners[1]]) - square (distances_w_s[winners[0]]), gng);

	// algorithm:09
	inc_neighbours_conn_age (winners[0], limit_network_size, gng);

	// algorithm:10: set connection to 0 (*initial-connection-age*) between two winners
	set_neuron_conn_age (winners[0], winners[1], INITIAL_CONNECTION_AGE, gng);

	// algorithm:11.a
	remove_old_conn_age (limit_conn_age, limit_network_size, gng);

	// algorithm:11.b
	find_and_del_neuron_with_min_utility_factor (k_utility, limit_network_size, gng);

	// algorithm:12
	if ((epoch % lambda_step == 0) &&
	    (limit_network_size > length_gng (limit_network_size, gng))) {
		adaptive_step_create_new_neuron (eps_local_error, dimension_of_sensor, limit_network_size, gng);
	}

	// algorithm:20,21
	decrease_all_neuron_local_errors_and_utility_factor (factor_beta_decrease_local_error, limit_network_size, gng);
}


/* return group number if exist */
// fixme: need more unit-testings
int recursive_search_group (int neuron_a, int group_number, int limit_network_size, NEURON *gng)
{
	int search_from = (group_number == NOT_IN_ANY_GROUPS) ? 0 : neuron_a + 1;

	for (int i=search_from; i<limit_network_size; i++) {
		if (gng[neuron_a].conn_age[i] >= INITIAL_CONNECTION_AGE) {
			group_number = recursive_search_group (i, i, limit_network_size, gng);
			gng[i].group = group_number;
		}
	}
	return group_number;
}



/* fixme: need clean group number:
   if neuron or group disconnected from existed group
   OR
   if two neurons or groups has connected
*/
void extract_groups_from_conn_ages (int limit_network_size, NEURON *gng)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON && gng[i].group == NOT_IN_ANY_GROUPS) {
			gng[i].group = recursive_search_group(i, NOT_IN_ANY_GROUPS, limit_network_size, gng);
			if (gng[i].group == NOT_IN_ANY_GROUPS) {gng[i].group = i;}
		}
	}
}



/* return 0 if all good */
int write_gng_to_file (char *file_name, int dimension_of_sensor, int limit_network_size, NEURON *gng)
{
	FILE * ifp;
	size_t result;
	int sum_res = 0;

	ifp = fopen(file_name, "wb");
	if (ifp != NULL) {
		for (int i=0; i<limit_network_size; i++) {
			result = fwrite(&(gng[i]), sizeof(struct Neuron), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
			result = fwrite(gng[i].weight, dimension_of_sensor * sizeof (* (gng[i].weight)), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
			result = fwrite(gng[i].conn_age, limit_network_size * sizeof (* (gng[i].conn_age)), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
		}
		fclose(ifp);
	}
	return (sum_res == 3 * limit_network_size) ? 0 : 1; /* 3 = gng + weight + conn_age */
}


/* return 0 if all good */
int read_gng_from_file (char *file_name, int dimension_of_sensor, int limit_network_size, NEURON *gng)
{
	FILE * ifp;
	size_t result;
	int sum_res = 0;

	ifp = fopen(file_name, "rb");
	if (ifp != NULL) {
		for (int i=0; i<limit_network_size; i++) {
			result = fread(&(gng[i]), sizeof(struct Neuron), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
			result = fread(gng[i].weight, dimension_of_sensor * sizeof (* (gng[i].weight)), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
			result = fread(gng[i].conn_age, limit_network_size * sizeof (* (gng[i].conn_age)), 1, ifp);
			sum_res += (result == 1) ? 1 : 0;
		}
		fclose(ifp);
	}
	return (sum_res == 3 * limit_network_size) ? 0 : 1; /* 3 = gng + weight + conn_age */
}
