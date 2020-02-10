/*
   example.c ---  simple usage of growing-neural-gas

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

#include "example.h"
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


	float example_sensor[DIMENSION_OF_SENSOR] = {10.0, 20.0, 30.0, 40.0};

	printf("empty network: (%d elements)\n", LIMIT_NETWORK_SIZE);
	initialization (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng);

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	for (int i=0; i<6; i++) { // add only 6 neurons
		add_neuron (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng);
	}
	/* very strange: copy random numbers from lisp for test algorithm */
	testing_gng[0].weight[0] = 3.899109226504383E-2;
	testing_gng[0].weight[1] = -0.5887572368130448;
	testing_gng[0].weight[2] = 0.15798545448641646;
	testing_gng[0].weight[3] = -0.3935485254550847;

	testing_gng[1].weight[0] = -2.5362783233591757E-2;
	testing_gng[1].weight[1] = -1.7387463358799848;
	testing_gng[1].weight[2] = 0.8288844096916685;
	testing_gng[1].weight[3] = 0.81767336357299;

	testing_gng[2].weight[0] = 0.34832968486910665;
	testing_gng[2].weight[1] = -0.8975345961191783;
	testing_gng[2].weight[2] = 0.2172056548774863;
	testing_gng[2].weight[3] = -1.1310410332299918;

	testing_gng[3].weight[0] = 0.43696231568422167;
	testing_gng[3].weight[1] = -1.217729705511355;
	testing_gng[3].weight[2] = 0.48712565439742406;
	testing_gng[3].weight[3] = -0.6556663212690108;

	testing_gng[4].weight[0] = -0.5163679995220464;
	testing_gng[4].weight[1] = 0.1978095514052677;
	testing_gng[4].weight[2] = 0.7042490286855793;
	testing_gng[4].weight[3] = 0.500058415939104;

	testing_gng[5].weight[0] = -1.491557671831566;
	testing_gng[5].weight[1] = 0.49370422828611404;
	testing_gng[5].weight[2] = 1.6056498050450605;
	testing_gng[5].weight[3] = 1.4681365733934029;

	printf ("\nsimple 6 neurons (all disconnected):\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	printf ("\nupdate connection age:\n");
	update_neuron_conn_age (0, 1, 1, testing_gng); /* need create link beetwin first neuron! */
	update_neuron_conn_age (1, 2, 2, testing_gng);
	update_neuron_conn_age (2, 0, 3, testing_gng);
	update_neuron_conn_age (3, 4, 4, testing_gng);

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	growing_neural_gas (0, EPS_WINNER, EPS_NEIGHBOUR, EPS_LOCAL_ERROR, FACTOR_BETA_DECREASE_LOCAL_ERROR, LIMIT_CONN_AGE, K_UTILITY, LAMBDA_STEP, NULL, example_sensor, DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng);

	printf ("\n\n\nResult of one step working \"growing neural gas\":\n");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (DIMENSION_OF_SENSOR, LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		free (testing_gng[i].weight);
		free (testing_gng[i].conn_age);
	}
	free (testing_gng);

	return 0;
}
