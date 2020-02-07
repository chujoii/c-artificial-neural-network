/*
   gng-to-dot_unit-testing.c ---  Unit testing for growing-neural-gas visualization with graphviz

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
#include "../../../gng-to-dot.h"

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

	NEURON *testing_gng = malloc(LIMIT_NETWORK_SIZE * sizeof(* testing_gng));
	if (testing_gng == NULL) {
		return 1;
	}
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		testing_gng[i].weight = malloc(DIMENSION_OF_SENSOR * sizeof (* (testing_gng[i].weight)));
		if (testing_gng[i].weight == NULL) {
			// fixme: free (testing_gng); and already allocation weights and conn_ages
			return 2;
		}

		testing_gng[i].conn_age = malloc(LIMIT_NETWORK_SIZE * sizeof (* (testing_gng[i].conn_age)));
		if (testing_gng[i].conn_age == NULL) {
			// fixme: free (testing_gng); and already allocation weights and conn_ages
			return 2;
		}
	}



	float example_sensor[DIMENSION_OF_SENSOR] = {0, 0, 5, 7};
	float distances[LIMIT_NETWORK_SIZE];
	int result;

	initialization (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng);
	for (int i=0; i<7; i++) { // add only 7 neurons
		add_neuron (LIMIT_NETWORK_SIZE, testing_gng);
	}



	/* very strange: copy random numbers from lisp for test algorithm */
	testing_gng[0].weight[0] = -9.8;
	testing_gng[0].weight[1] = -19.6;
	testing_gng[0].weight[2] = -29.4;
	testing_gng[0].weight[3] = -39.2;

	testing_gng[1].weight[0] = -7.8;
	testing_gng[1].weight[1] = -17.6;
	testing_gng[1].weight[2] = -27.4;
	testing_gng[1].weight[3] = -37.2;

	testing_gng[2].weight[0] = 2.1;
	testing_gng[2].weight[1] = 2.2;
	testing_gng[2].weight[2] = 2.3;
	testing_gng[2].weight[3] = 2.4;

	testing_gng[3].weight[0] = -65.9;
	testing_gng[3].weight[1] = -164.8;
	testing_gng[3].weight[2] = -263.7;
	testing_gng[3].weight[3] = -362.6;

	testing_gng[4].weight[0] = -1.8;
	testing_gng[4].weight[1] = -11.6;
	testing_gng[4].weight[2] = -21.4;
	testing_gng[4].weight[3] = -31.2;

	testing_gng[5].weight[0] = 5.1;
	testing_gng[5].weight[1] = 5.2;
	testing_gng[5].weight[2] = 5.3;
	testing_gng[5].weight[3] = 5.4;

	testing_gng[6].weight[0] = -3.9;
	testing_gng[6].weight[1] = -8.7;
	testing_gng[6].weight[2] = -13.5;
	testing_gng[6].weight[3] = -18.4;

	set_neuron_conn_age (0, 1, 1, testing_gng);
	set_neuron_conn_age (0, 6, 3, testing_gng);
	set_neuron_conn_age (1, 2, 0, testing_gng);
	set_neuron_conn_age (2, 6, 3, testing_gng);
	set_neuron_conn_age (3, 4, 4, testing_gng);

	update_neuron_local_error (0, 1.0, testing_gng);
	update_neuron_local_error (1, 2.0, testing_gng);
	update_neuron_local_error (2, 3.0, testing_gng);
	update_neuron_local_error (3, 4.0, testing_gng);
	update_neuron_local_error (4, 5.0, testing_gng);
	update_neuron_local_error (5, 6.0, testing_gng);
	update_neuron_local_error (6, 7.0, testing_gng);

	update_neuron_utility_factor (0, 0.45, testing_gng);
	update_neuron_utility_factor (1, 0.5, testing_gng);
	update_neuron_utility_factor (2, 0.4, testing_gng);
	update_neuron_utility_factor (3, 0.2, testing_gng);
	update_neuron_utility_factor (4, 0.3, testing_gng);
	update_neuron_utility_factor (5, 0.7, testing_gng);
	update_neuron_utility_factor (6, 0.45, testing_gng);

	printf ("\nsimple 7 neurons:\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	convert_gng_conn_ages_to_simple_list (LIMIT_NETWORK_SIZE, testing_gng, stdout);

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		free (testing_gng[i].weight);
		free (testing_gng[i].conn_age);
	}
	free (testing_gng);

	return 0;
}
