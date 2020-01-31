/* 
   growing-neural-gas.c --- Simple library for "growing neural gas with utility" algorithm
   "https://ru.wikipedia.org/wiki/Нейронный_газ"
   "https://en.wikipedia.org/wiki/Neural_gas"

   based on main article:
   "http://www.ks.uiuc.edu/Publications/Papers/PDF/MART91B/MART91B.pdf"
   Thomas Martinetz and Klaus Schulten (1991).
   "A "neural gas" network learns topologies".
   Artificial Neural Networks. Elsevier. pp. 397–402.
   
   Copyright (C) 2019 Roman V. Prikhodchenko
   
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



void initialization (NEURON *gng)
{
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		gng[i].active = OFF;
		/* fixme: memset for float? */
		for (int j=0; j<DIMENSION_OF_SENSOR; j++) gng[i].weight[j] = 0.0;
		for (int j=0; j<LIMIT_NETWORK_SIZE; j++) gng[i].conn_age[j] = NOT_CONNECTED;
		gng[i].local_error = 0.0;
		gng[i].utility_factor = 0.0;
	}
}



int print_neuron (NEURON neuron)
{
	if (neuron.active == OFF) {
		printf("-\n");
		return 0;
	}

	printf("w:");
	for (int i=0; i<DIMENSION_OF_SENSOR; i++) {
		printf(" %7.1f", neuron.weight[i]);
	}

	printf("\ta:");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (neuron.conn_age[i] < INITIAL_CONNECTION_AGE)
			printf (" -");
		else
			printf(" %d", neuron.conn_age[i]);
	}

	printf("\te: %5.3f\t", neuron.local_error);
	printf("u: %5.3f\n", neuron.utility_factor);

	return 0;
}



// fixme: print-gng-as-list (gng)



int add_neuron (NEURON *gng)
{
	int index = -1;
	int i = 0;
	while ((gng[i].active == ON) && i < LIMIT_NETWORK_SIZE) {i++;}
	if (i < LIMIT_NETWORK_SIZE) {
		gng[i].active = ON;
		index = i;
	}
	return index;
}



void find_and_del_neuron_with_min_utility_factor (float k, NEURON *gng)
{
	float E_median = value_of_median_local_error (gng);
	int index_of_U_min = index_of_minimum_utility_factor (gng);

	if (length_gng (gng) > 2    &&
	    (E_median / gng[index_of_U_min].utility_factor) > k) {
		printf("[d]Emedian=%f Umin[%d]=%f    Emedian/Umin=%f > k=%f\n", E_median, index_of_U_min, gng[index_of_U_min].utility_factor, E_median / gng[index_of_U_min].utility_factor, k);
		/* delete-neuron-U-min */
		printf("remove neuron number %d\n", index_of_U_min);
		gng[index_of_U_min].active = OFF;

		/* make-consistent-gng */
		/* remove-unexisted-conn-age */
		for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
			if (gng[i].active == ON) {
				gng[i].conn_age[index_of_U_min] = NOT_CONNECTED;
			}
		}

		reconnect (gng); // fixme: really need?
	}
}



int index_of_minimum_utility_factor (NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = 0;
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
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



int length_gng (NEURON *gng)
{
	int len = 0;
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
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
int index_of_median_local_error (NEURON *gng)
{ // fixme: remove function?
	LOCALERROR lerr_arr[LIMIT_NETWORK_SIZE];
	int counter = 0;

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
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
float value_of_median_local_error (NEURON *gng)
{
	float lerr_arr[LIMIT_NETWORK_SIZE];
	int counter = 0;

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
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
void reconnect (NEURON *gng)
{
	int len = length_gng (gng);
	int neuron_a = -1;
	int neuron_b = -1;

	if (len <= 2) {
		for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
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



void update_neuron_weight_vector(int neuron_a, float step, float *sensor, NEURON *gng)
{
	for (int i=0; i<DIMENSION_OF_SENSOR; i++) {
		gng[neuron_a].weight[i] += step*(gng[neuron_a].weight[i] - sensor[i]);
	}
}



void update_neighbours_weights (int neuron_a, float eps_step, float *sensor, NEURON *gng)
{
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (gng[neuron_a].conn_age[i] >= INITIAL_CONNECTION_AGE) {
			update_neuron_weight_vector (i, eps_step, sensor, gng);
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
void inc_neighbours_conn_age (int neuron_a, NEURON *gng)
{
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (gng[neuron_a].conn_age[i] >= INITIAL_CONNECTION_AGE) {
			update_neuron_conn_age (neuron_a, i, 1, gng);
		}
	}
}



void remove_old_conn_age (int limit_conn_age, NEURON *gng)
{
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (gng[i].active == ON) {
			for (int j=i; j<LIMIT_NETWORK_SIZE; j++) {
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



// fixme: decrease-all-neuron-local-errors-and-utility-factor (factor-beta gng)



void calculate_distance_weight_sensor (float *sensor, NEURON *gng, float *return_distance)
{
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (gng[i].active == ON ) {
			return_distance[i] = euclidean_distance_vector (gng[i].weight, sensor, DIMENSION_OF_SENSOR);
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
void calculate_distance_in_mixed_space_weight_sensor (int *mixed_space, float *sensor, NEURON *gng, float *return_distance)
{
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (gng[i].active == ON ) {
			float sum = 0.0;
			for (int j=0; j<DIMENSION_OF_SENSOR; j++) {
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
int find_neuron_index_with_max_local_error (NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = 0;
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
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
int find_neighbours_index_with_max_local_error (int index_max_local_error, NEURON *gng)
{
	int counter = 0;
	float value = 0.0;
	int index = -1;
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
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



void adaptive_step_create_new_neuron (NEURON *gng)
{
	int index_neuron_max_local_error = find_neuron_index_with_max_local_error (gng); // algorithm:13
	int index_neighbour_for_max_local_error = find_neighbours_index_with_max_local_error (index_neuron_max_local_error, gng); // algorithm:14
	int index_of_new_neuron = add_neuron(gng); // algorithm:15.a

	if (index_neighbour_for_max_local_error < 0) { // fix situation when all index = -1
		// more correct solution: use not aggressive coefficients (k_utility)
		for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
			if ((gng[i].active == ON) &&
			    index_neighbour_for_max_local_error != index_neuron_max_local_error) { index_neighbour_for_max_local_error = i; }
		}
	}

	// fixme: move "algorithm:15.b" to function update_neuron_weight_vector
	// algorithm:15.b
	for (int i=0; i<DIMENSION_OF_SENSOR; i++) {
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
	gng[index_neuron_max_local_error].local_error *= EPS_LOCAL_ERROR;
	gng[index_neighbour_for_max_local_error].local_error *= EPS_LOCAL_ERROR;
	gng[index_of_new_neuron].local_error = gng[index_neuron_max_local_error].local_error;
}



// fixme: growing-neural-gas epoch sensor (gng)
