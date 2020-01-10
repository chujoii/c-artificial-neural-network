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

	printf("\te: %5.1f\t", neuron.local_error);
	printf("u: %5.1f\n", neuron.utility_factor);

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



// fixme: find-and-del-neuron-with-min-utility-factor (k gng)



void update_neuron_weight_vector(int neuron_a, float step, float *sensor, NEURON *gng)
{
	for (int i=0; i<DIMENSION_OF_SENSOR; i++) {
		gng[neuron_a].weight[i] = gng[neuron_a].weight[i] + step*(gng[neuron_a].weight[i] - sensor[i]);
	}
}



// fixme: update-neighbours-weights (function list-neighbour eps-step gng)



void update_neuron_conn_age (int neuron_a, int neuron_b, int step, NEURON *gng)
{
	gng[neuron_a].conn_age[neuron_b] += step;
	gng[neuron_b].conn_age[neuron_a] += step;
}



void disconnect_neuron (int neuron_a, int neuron_b, NEURON *gng)
{
	gng[neuron_a].conn_age[neuron_b] = NOT_CONNECTED;
	gng[neuron_b].conn_age[neuron_a] = NOT_CONNECTED;
}


// fixme: inc-neighbours-conn-age (a gng)
// fixme: remove-old-conn-age (limit-conn-age gng)





void update_neuron_local_error (int neuron_a, float step, NEURON *gng)
{
	gng[neuron_a].local_error += step;
}


// fixme: update-neuron-utility-factor (a function step gng)
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



// fixme: find-index-of-two-minimal (in-list)
// fixme: find-neuron-index-with-max-local-error (gng)
// fixme: find-neighbours-index-with-max-local-error (index-max-local-error gng)
// fixme: adaptive-step-create-new-neuron (gng)
// fixme: growing-neural-gas epoch sensor (gng)
