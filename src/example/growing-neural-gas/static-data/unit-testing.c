/* 
   unit-testing.c ---  Unit testing for growing-neural-gas code

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
#include <stdlib.h>

#include "unit-testing.h"
#include "../../../growing-neural-gas.h"
#include "../../../vector.h"

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

	GNG_NEURON *testing_gng = malloc(GNG_LIMIT_NETWORK_SIZE * sizeof(* testing_gng));
	if (testing_gng == NULL) {
		return 1;
	}
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		testing_gng[i].weight = malloc(GNG_DIMENSION_OF_SENSOR * sizeof (* (testing_gng[i].weight)));
		if (testing_gng[i].weight == NULL) {
			// fixme: free (testing_gng); and already allocation weights and conn_ages
			return 2;
		}

		testing_gng[i].conn_age = malloc(GNG_LIMIT_NETWORK_SIZE * sizeof (* (testing_gng[i].conn_age)));
		if (testing_gng[i].conn_age == NULL) {
			// fixme: free (testing_gng); and already allocation weights and conn_ages
			return 2;
		}
	}


	GNG_NEURON *testing2_gng = malloc(GNG_LIMIT_NETWORK_SIZE * sizeof(* testing2_gng));
	if (testing2_gng == NULL) {
		return 1;
	}
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		testing2_gng[i].weight = malloc(GNG_DIMENSION_OF_SENSOR * sizeof (* (testing2_gng[i].weight)));
		if (testing2_gng[i].weight == NULL) {
			// fixme: free (testing2_gng); and already allocation weights and conn_ages
			return 2;
		}

		testing2_gng[i].conn_age = malloc(GNG_LIMIT_NETWORK_SIZE * sizeof (* (testing2_gng[i].conn_age)));
		if (testing2_gng[i].conn_age == NULL) {
			// fixme: free (testing2_gng); and already allocation weights and conn_ages
			return 2;
		}
	}

	float example_sensor[GNG_DIMENSION_OF_SENSOR] = {10, 20, 30, 40};
	float example_sensor_with_angle[GNG_DIMENSION_OF_SENSOR] = {10,     20,     350,   40};
	int mixed_space_distances[GNG_DIMENSION_OF_SENSOR] ={GNG_EUCLIDEAN, GNG_EUCLIDEAN, GNG_ANGLE, GNG_EUCLIDEAN};
	float distances[GNG_LIMIT_NETWORK_SIZE];
	int result;

	printf("empty network: (%d elements)\n", GNG_LIMIT_NETWORK_SIZE);


	initialization (GNG_EPS_WINNER, GNG_EPS_NEIGHBOUR, GNG_EPS_LOCAL_ERROR, GNG_FACTOR_BETA_DECREASE_LOCAL_ERROR, GNG_LIMIT_CONN_AGE, GNG_K_UTILITY, GNG_LAMBDA_STEP, GNG_LIMIT_NETWORK_SIZE, GNG_DIMENSION_OF_SENSOR, testing_gng);
	extract_groups_from_conn_ages (testing_gng); // add extract_groups_from_conn_ages before print_neuron for update group list (only visual changes)
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	printf("\nadd neuron: number=%d\n", add_gng_neuron (testing_gng));
	printf("add neuron: number=%d\n", add_gng_neuron (testing_gng));

	printf("\nconnect neurons 0 and 1\n");
	printf("weight\t\t\t\t\tconn-age\t\tlocal-error\tutility-factor\n");
	update_neuron_conn_age (0, 1, 1, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	set_neuron_conn_age (0, 1, GNG_NOT_CONNECTED, testing_gng);
	printf ("\nsimple 6 neurons (all disconnected):\n");
	add_gng_neuron (testing_gng);add_gng_neuron (testing_gng);add_gng_neuron (testing_gng);add_gng_neuron (testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}


	printf ("\nmanual change weight (random:normal too hard for human readable):\n");
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		for (int j = 0; j < GNG_DIMENSION_OF_SENSOR; j++) {
			testing_gng[i].weight[j] =i + (j + 1)/10.0;
		}

		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	printf ("\nupdate weight neuron number 3 (new_weight_vector = weight_vector + eps*(veight_vector - sensor_vector)):\n");
	update_neuron_weight_vector(3, GNG_EPS_WINNER, example_sensor, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	printf ("\nupdate connection age:\n");
	update_neuron_conn_age (0, 1, 1, testing_gng); /* need create link beetwin first neuron! */
	update_neuron_conn_age (1, 2, 2, testing_gng);
	update_neuron_conn_age (2, 0, 3, testing_gng);
	update_neuron_conn_age (3, 4, 4, testing_gng);

	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}


	update_neuron_local_error (0, 0.9, testing_gng);
	update_neuron_local_error (1, 0.5, testing_gng);
	update_neuron_local_error (2, 0.8, testing_gng);
	update_neuron_local_error (3, 0.2, testing_gng);
	update_neuron_local_error (4, 0.3, testing_gng);
	update_neuron_local_error (5, 0.7, testing_gng);
	printf ("\nupdate local error (last column):\n");
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}


	printf ("\nCalculate distance between Weight (neuron number 3) and Sensor (");
	for (int j = 0; j < GNG_DIMENSION_OF_SENSOR; j++) {
		printf (" %7.2f", example_sensor[j]);
	}
	printf (")");
	printf (":\n(%7.2f %7.2f %7.2f %7.2f ...) compare with:\n",
		euclidean_distance_vector (testing_gng[0].weight, example_sensor, GNG_DIMENSION_OF_SENSOR),
		euclidean_distance_vector (testing_gng[1].weight, example_sensor, GNG_DIMENSION_OF_SENSOR),
		euclidean_distance_vector (testing_gng[2].weight, example_sensor, GNG_DIMENSION_OF_SENSOR),
		euclidean_distance_vector (testing_gng[3].weight, example_sensor, GNG_DIMENSION_OF_SENSOR));
	calculate_distance_weight_sensor (example_sensor, testing_gng, distances);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		if (testing_gng[i].active == GNG_ON ) {
			printf (" %7.2f", distances[i]);
		} else {printf (" NaN");}
	}
	
	printf ("\n\n\nsimple 3 neurons with angle:\n");
	initialization (GNG_EPS_WINNER, GNG_EPS_NEIGHBOUR, GNG_EPS_LOCAL_ERROR, GNG_FACTOR_BETA_DECREASE_LOCAL_ERROR, GNG_LIMIT_CONN_AGE, GNG_K_UTILITY, GNG_LAMBDA_STEP, GNG_LIMIT_NETWORK_SIZE, GNG_DIMENSION_OF_SENSOR, testing2_gng);
	add_gng_neuron (testing2_gng);add_gng_neuron (testing2_gng);add_gng_neuron (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		for (int j = 0; j < GNG_DIMENSION_OF_SENSOR; j++) {
			testing2_gng[i].weight[j] =i + (j + 1)/10.0;
		}
	}
	testing2_gng[0].weight[2] = 270.0;
	testing2_gng[1].weight[2] =  10.0;
	testing2_gng[2].weight[2] = 300.0;
	extract_groups_from_conn_ages (testing2_gng);
	for (int j = 0; j < GNG_LIMIT_NETWORK_SIZE; j++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[j]);
	}

	printf ("\nSensor with angle:");
	for (int j = 0; j < GNG_DIMENSION_OF_SENSOR; j++) {
		printf (" %7.2f", example_sensor_with_angle[j]);
	}

	printf ("\nMinimal distance will be for neuron number 1 (count from 0), because 10 degree nearest to 350 degree (distance=20 degree):\n");
	printf ("Distance between Weight (with angle) and Sensor (with angle) calculations (also with respect to angle):\n");
	calculate_distance_in_mixed_space_weight_sensor (mixed_space_distances, example_sensor_with_angle, testing2_gng, distances);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		if (testing2_gng[i].active == GNG_ON ) {
			printf (" %7.2f", distances[i]);
		} else {printf (" NaN");}
	}



	printf ("\n\nFind indexes of two minimal elementt (first correct answer), (then calculated answer)");

	float ti[3];
	int to[2]; // need only two elements
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
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}
	printf ("\nprint neighbour for neuron number 2:\n(number 0 (conn-age=2), number 1 (conn-age = 1), nc, nc, nc, nc):\n");
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		if (testing_gng[i].active == GNG_ON ) {
			printf (" %d", testing_gng[2].conn_age[i]);
		} else {printf (" -");}
	}
	printf ("\n\nupdate weight for this neurons (0 and 1):\n");
	update_neighbours_weights (2, GNG_EPS_NEIGHBOUR, example_sensor, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}


	printf ("\nprint neighbour for neuron number 3:\n(nc, nc, nc, nc, number 4 (conn-age = 3), nc):\n");
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		if (testing_gng[i].active == GNG_ON ) {
			printf (" %d", testing_gng[3].conn_age[i]);
		} else {printf (" -");}
	}
	printf ("\n\nupdate weight for this neuron (4):\n");
	update_neighbours_weights (3, GNG_EPS_NEIGHBOUR, example_sensor, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}



	printf ("\nincrease by 1 all connections form neuron print neighbour for winner (for example neuron number 0 (use different neuron only for example)):\n");
	inc_neighbours_conn_age (0, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}
	printf ("\nincrease by 1 all connections form neuron print neighbour (for example neuron number 3 (use different neuron only for example)):\n");
	inc_neighbours_conn_age (3, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}



	printf ("\nset connection to 0 (*initial-connection-age*) between two winners (1 2):\n");
	set_neuron_conn_age (1, 2, GNG_INITIAL_CONNECTION_AGE, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}



	printf ("\ncopy artificial neural network (because next two manipulation decrease size of net):\n");
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		testing2_gng[i].active = testing_gng[i].active;
		for (int j = 0; j < GNG_DIMENSION_OF_SENSOR; j++) testing2_gng[i].weight[j] = testing_gng[i].weight[j];
		for (int j = 0; j < GNG_LIMIT_NETWORK_SIZE; j++) testing2_gng[i].conn_age[j] = testing_gng[i].conn_age[j];
		testing2_gng[i].local_error = testing_gng[i].local_error;
		testing2_gng[i].utility_factor = testing_gng[i].utility_factor;
	}

	printf ("\nif age > limit (%d), then remove connection:\n", GNG_LIMIT_CONN_AGE);
	remove_old_conn_age (testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}


	printf ("\nset k-utility to %7.2f\n", GNG_K_UTILITY);
	printf ("remove neurons with min utility-factor (first element (if utility-factor equal)):\n");
	find_and_del_neuron_with_min_utility_factor (testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}

	printf ("repeat deletion:\n");
	find_and_del_neuron_with_min_utility_factor (testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}

	printf ("repeat deletion:\n");
	find_and_del_neuron_with_min_utility_factor (testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}

	printf ("repeat deletion:\n");
	find_and_del_neuron_with_min_utility_factor (testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}

	printf ("last repeat deletion (because size of network=2 and nothing will be deleted):\n");
	find_and_del_neuron_with_min_utility_factor (testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}



	printf ("\ncopy artificial neural network (because next two manipulation decrease size of net)\n");
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		testing2_gng[i].active = testing_gng[i].active;
		for (int j = 0; j < GNG_DIMENSION_OF_SENSOR; j++) testing2_gng[i].weight[j] = testing_gng[i].weight[j];
		for (int j = 0; j < GNG_LIMIT_NETWORK_SIZE; j++) testing2_gng[i].conn_age[j] = testing_gng[i].conn_age[j];
		testing2_gng[i].local_error = testing_gng[i].local_error;
		testing2_gng[i].utility_factor = testing_gng[i].utility_factor;
	}
	
	printf("set <<random>> utility factor:\n");
	update_neuron_utility_factor (0, 0.9, testing2_gng);
	update_neuron_utility_factor (1, 0.4, testing2_gng);
	update_neuron_utility_factor (2, 0.5, testing2_gng);
	update_neuron_utility_factor (3, 0.8, testing2_gng);
	update_neuron_utility_factor (4, 0.7, testing2_gng);
	update_neuron_utility_factor (5, 0.1, testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}
	
	printf ("\nremove neurons with min utility-factor:\n");
	find_and_del_neuron_with_min_utility_factor (testing2_gng);
	extract_groups_from_conn_ages (testing2_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing2_gng[i]);
	}


	printf ("\nrestore big artificial neural network\n");
	update_neuron_utility_factor (0, 0.9, testing_gng);
	update_neuron_utility_factor (1, 0.4, testing_gng);
	update_neuron_utility_factor (2, 0.5, testing_gng);
	update_neuron_utility_factor (3, 0.8, testing_gng);
	update_neuron_utility_factor (4, 0.7, testing_gng);
	update_neuron_utility_factor (5, 0.1, testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}


	int index_max_local_error = find_neuron_index_with_max_local_error (testing_gng);
	printf ("\nindex of neuron with max local error: %d\n", index_max_local_error);
	printf ("neighbour (for neuron number %d) index of neuron with max local error: %d\n",
		index_max_local_error,
		find_neighbours_index_with_max_local_error (index_max_local_error, testing_gng));


	printf ("\nadaptive step: create new neuron:\n");
	adaptive_step_create_new_neuron (testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}


	printf ("\ndecrease local-error and utility-factor:\n");
	decrease_all_neuron_local_errors_and_utility_factor (testing_gng);
	extract_groups_from_conn_ages (testing_gng);
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	printf("\ntest binary-formatted output: see file knowledge-base.gng\n");
	printf ("write GNG to file knowledge-base.gng ...");
	result = write_gng_to_file ("knowledge-base.gng", testing_gng);
	printf (" %s\n", (result == 0) ? "OK" : "Error");
	printf ("initialize (clean) gng\n");
	initialization (GNG_EPS_WINNER, GNG_EPS_NEIGHBOUR, GNG_EPS_LOCAL_ERROR, GNG_FACTOR_BETA_DECREASE_LOCAL_ERROR, GNG_LIMIT_CONN_AGE, GNG_K_UTILITY, GNG_LAMBDA_STEP, GNG_LIMIT_NETWORK_SIZE, GNG_DIMENSION_OF_SENSOR, testing_gng);
	printf ("read GNG from file knowledge-base.gng ...");
	result = read_gng_from_file ("knowledge-base.gng", testing_gng);
	printf (" %s (%d)\n", (result == 0) ? "OK" : "Error", result);
	// not need use  extract_groups_from_conn_ages, because nothing changes with GNG
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}



	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		free (testing_gng[i].weight);
		free (testing_gng[i].conn_age);
	}
	free (testing_gng);

	return 0;
}
