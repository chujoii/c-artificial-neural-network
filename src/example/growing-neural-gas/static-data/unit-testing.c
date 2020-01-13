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
	float example_sensor_with_angle[DIMENSION_OF_SENSOR] = {10,     20,     350,   40};
	int mixed_space_distances[DIMENSION_OF_SENSOR] ={EUCLIDEAN, EUCLIDEAN, ANGLE, EUCLIDEAN};
	NEURON example_gng_with_angle[LIMIT_NETWORK_SIZE];
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
	update_neuron_conn_age (0, 1, 1, testing_gng);
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
	
	printf ("\n\n\nsimple 3 neurons with angle:\n");
	initialization (example_gng_with_angle);
	add_neuron(example_gng_with_angle);add_neuron(example_gng_with_angle);add_neuron(example_gng_with_angle);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		for (int j=0; j<DIMENSION_OF_SENSOR; j++) {
			example_gng_with_angle[i].weight[j] =i + (j + 1)/10.0;
		}
	}
	example_gng_with_angle[0].weight[2] = 270.0;
	example_gng_with_angle[1].weight[2] =  10.0;
	example_gng_with_angle[2].weight[2] = 300.0;
	for (int j=0; j<LIMIT_NETWORK_SIZE; j++) {
		print_neuron (example_gng_with_angle[j]);
	}

	printf ("\nSensor with angle:");
	for (int j=0; j<DIMENSION_OF_SENSOR; j++) {
		printf (" %7.2f", example_sensor_with_angle[j]);
	}

	printf ("\nMinimal distance will be for neuron number 1 (count from 0), because 10 degree nearest to 350 degree (distance=20 degree):\n");
	printf ("Distance between Weight (with angle) and Sensor (with angle) calculations (also with respect to angle):\n");
	calculate_distance_in_mixed_space_weight_sensor (mixed_space_distances, example_sensor_with_angle, example_gng_with_angle, distances);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (example_gng_with_angle[i].active == ON ) {
			printf (" %7.2f", distances[i]);
		} else {printf (" NaN");}
	}



	printf ("\n\nFind indexes of two minimal elementt (first correct answer), (then calculated answer)");

	float ti[3];
	int to[3];
	int ts;

	ts = 2;
	ti[0] = 1; ti[1] = 1;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(0 1) (%d %d)", to[0], to[1]);

	ts = 2;
	ti[0] = 1; ti[1] = 2;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(0 1) (%d %d)", to[0], to[1]);

	ts = 2;
	ti[0] = 2; ti[1] = 1;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(1 0) (%d %d)", to[0], to[1]);

	ts = 3;
	ti[0] = 1; ti[1] = 1; ti[2] = 2;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(0 1) (%d %d)", to[0], to[1]);
	
	ts = 3;
	ti[0] = 1; ti[1] = 2; ti[2] = 1;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(0 2) (%d %d)", to[0], to[1]);
	
	ts = 3;
	ti[0] = 2; ti[1] = 1; ti[2] = 1;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(1 2) (%d %d)", to[0], to[1]);
	
	ts = 3;
	ti[0] = 1; ti[1] = 2; ti[2] = 3;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(0 1) (%d %d)", to[0], to[1]);

	ts = 3;
	ti[0] = 1; ti[1] = 3; ti[2] = 2;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(0 2) (%d %d)", to[0], to[1]);

	ts = 3;
	ti[0] = 2; ti[1] = 1; ti[2] = 3;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(1 0) (%d %d)", to[0], to[1]);

	ts = 3;
	ti[0] = 2; ti[1] = 3; ti[2] = 1;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(2 0) (%d %d)", to[0], to[1]);

	ts = 3;
	ti[0] = 3; ti[1] = 1; ti[2] = 2;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(1 2) (%d %d)", to[0], to[1]);

	ts = 3;
	ti[0] = 3; ti[1] = 2; ti[2] = 1;
	find_index_of_two_minimal (ti, ts, to);
	printf ("\n(2 1) (%d %d)", to[0], to[1]);



	printf ("\n\nsimple 6 neurons (repeat):\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}
	printf ("\nprint neighbour for neuron number 2:\n(number 0 (conn-age=2), number 1 (conn-age = 1), nc, nc, nc, nc):\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (testing_gng[i].active == ON ) {
			printf (" %d", testing_gng[2].conn_age[i]);
		} else {printf (" -");}
	}
	printf ("\n\nupdate weight for this neurons (0 and 1):\n");
	update_neighbours_weights (2, EPS_NEIGHBOUR, example_sensor, testing_gng);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}


	printf ("\nprint neighbour for neuron number 3:\n(nc, nc, nc, nc, number 4 (conn-age = 3), nc):\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		if (testing_gng[i].active == ON ) {
			printf (" %d", testing_gng[3].conn_age[i]);
		} else {printf (" -");}
	}
	printf ("\n\nupdate weight for this neuron (4):\n");
	update_neighbours_weights (3, EPS_NEIGHBOUR, example_sensor, testing_gng);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}



	return 0;
}
