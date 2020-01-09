/* 
   unit-testing.c ---  Unit testing for growing-neural-gas code

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

#include "unit-testing.h"
//#include "../../../growing-neural-gas.h" # fixme: uncomment? //extern const int DIMENSION_OF_SENSOR; extern const int LIMIT_NETWORK_SIZE;
#include "../../../vector.h"
#include "../../../growing-neural-gas.c" // fixme

/* Use  for debug print, or #f for silent */
//#define DEBUG
// uncomment previous string for debug=ON
#ifdef DEBUG
 #define DEBUG_PRINT(x)  fprintf (stderr, x)
#else
 #define DEBUG_PRINT(x)
#endif



int main ()
{
	float example_sensor[DIMENSION_OF_SENSOR] = {10, 20, 30, 40};
	NEURON testing_gng[LIMIT_NETWORK_SIZE];
	float distances[LIMIT_NETWORK_SIZE];

	printf("empty network: (%d elements)\n", LIMIT_NETWORK_SIZE);
	initialization (testing_gng);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}

	printf("\nadd neuron: number=%d\n", add_neuron(testing_gng));
	printf("add neuron: number=%d\n", add_neuron(testing_gng));

	printf("\nconnect neurons 0 and 1\n");
	printf("weight\t\t\t\t\tconn-age\t\tlocal-error\tutility-factor\n");
	inc_neuron_conn_age (0, 1, 1, testing_gng);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}

	disconnect_neuron(0, 1, testing_gng);
	printf ("\nsimple 6 neurons (all disconnected):\n");
	add_neuron(testing_gng);add_neuron(testing_gng);add_neuron(testing_gng);add_neuron(testing_gng);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}


	printf ("\nmanual change weight (random:normal too hard for human readable):\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		for (int j=0; j<DIMENSION_OF_SENSOR; j++) {
			testing_gng[i].weight[j] =i + (j + 1)/10.0;
		}

		print_neuron (testing_gng[i]);
	}

	printf ("\nupdate weight neuron number 3 (new_weight_vector = weight_vector + eps*(veight_vector - sensor_vector)):\n");
	update_neuron_weight_vector(3, EPS_WINNER, example_sensor, testing_gng);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}

	printf ("\nupdate connection age:\n");
	update_neuron_conn_age (0, 1, 1, testing_gng); /* need create link beetwin first neuron! */
	update_neuron_conn_age (1, 2, 2, testing_gng);
	update_neuron_conn_age (2, 0, 3, testing_gng);
	update_neuron_conn_age (3, 4, 4, testing_gng);

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}


	update_neuron_local_error (0, 0.9, testing_gng);
	update_neuron_local_error (1, 0.5, testing_gng);
	update_neuron_local_error (2, 0.8, testing_gng);
	update_neuron_local_error (3, 0.2, testing_gng);
	update_neuron_local_error (4, 0.3, testing_gng);
	update_neuron_local_error (5, 0.7, testing_gng);
	printf ("\nupdate local error (last column):\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}


	printf ("\nCalculate distance between Weight (neuron number 3) and Sensor (");
	for (int j=0; j<DIMENSION_OF_SENSOR; j++) {
		printf (" %7.2f", example_sensor[j]);
	}
	printf (")");
	printf (":\n(%7.2f %7.2f %7.2f %7.2f ...) compare with:\n",
		euclidean_distance_vector (testing_gng[0].weight, example_sensor, DIMENSION_OF_SENSOR),
		euclidean_distance_vector (testing_gng[1].weight, example_sensor, DIMENSION_OF_SENSOR),
		euclidean_distance_vector (testing_gng[2].weight, example_sensor, DIMENSION_OF_SENSOR),
		euclidean_distance_vector (testing_gng[3].weight, example_sensor, DIMENSION_OF_SENSOR));
	calculate_distance_weight_sensor (example_sensor, testing_gng, distances);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (testing_gng[i].active == ON ) {
			printf (" %7.2f", distances[i]);
		} else {printf (" NaN");}
	}
	printf ("\n");

	return 0;
}
