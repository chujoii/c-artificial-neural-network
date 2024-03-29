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

/*
  Keywords: algorithm neuron network machine learning growing neural gas visualization graphviz
*/



/* Usage: (instead of spring model of graphviz "neato", you can use "fdp" or "sfdp") (instead of "feh" any image and graphic viewer)
   make
   ./gng-to-dot_unit-testing
   neato -Tpng -O test.gv
   feh test.gv.png
*/

#include <stdio.h>
#include <stdlib.h>

#include "unit-testing.h"
#include "../../../growing-neural-gas.h"
#include "../../../vector.h"
#include "../../../gng-to-dot.h"
#include "gng-to-dot_unit-testing.h"

/* Use  for debug print, or #f for silent */
//#define DEBUG
// uncomment previous string for debug=ON
#ifdef DEBUG
 #define DEBUG_PRINT(x)  fprintf (stderr, x)
#else
 #define DEBUG_PRINT(x)
#endif


#define GNG_COLOR_LEN 11
char * color_list [] = { //list based on https://en.wikipedia.org/wiki/Web_colors
	//"White",
	//"Silver", // similar to Gray
	"Gray",
	//"Black", // fixme: need change font color for contrast
	//"Red", // already used for border (border=red if value out of limit)
	"Maroon",
	"Yellow",
	"Olive",
	"Lime",
	//"Green", // already used for border (border=green if value inside limit)
	"Aqua",
	"Teal",
	"Blue",
	"Navy",
	"Fuchsia",
	"Purple"};


void fill_limits (int dimension_of_sensor, float limits_of_weight[][2]);

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
	int winners[2];
	float example_sensor[GNG_DIMENSION_OF_SENSOR] = {10, 20, 30, 40};
	float limits_of_weight[GNG_DIMENSION_OF_SENSOR][2];

	initialization (GNG_EPS_WINNER, GNG_EPS_NEIGHBOUR, GNG_EPS_LOCAL_ERROR, GNG_FACTOR_BETA_DECREASE_LOCAL_ERROR, GNG_LIMIT_CONN_AGE, GNG_K_UTILITY, GNG_LAMBDA_STEP, GNG_LIMIT_NETWORK_SIZE, GNG_DIMENSION_OF_SENSOR, testing_gng);
	for (int i = 0; i < 7; i++) { // add only 7 neurons
		add_gng_neuron (testing_gng);
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

	winners[0] = 1;
	winners[1] = 0;

	fill_limits (GNG_DIMENSION_OF_SENSOR, limits_of_weight);
	
	extract_groups_from_conn_ages (testing_gng);

	printf ("\nsimple 7 neurons:\n");
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}

	printf("remove group number manually from neuron number 1:\n");
	testing_gng[1].group = GNG_NOT_IN_ANY_GROUPS;

	printf ("see group for neuron number 1:\n");
	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}
	printf ("\ngenerate group number again:\n");
	extract_groups_from_conn_ages (testing_gng);

	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		print_gng_neuron (GNG_DIMENSION_OF_SENSOR, GNG_LIMIT_NETWORK_SIZE, testing_gng[i]);
	}


	printf ("\nconn_ages as simple list:\n");
	convert_gng_conn_ages_to_simple_list (GNG_MIN_CONN_WIDTH, GNG_MAX_CONN_WIDTH, testing_gng, stdout);

	printf ("\n\nwrite GNG to DOT-formatted (graphviz) file ...\n");
	gng_to_dot_file ("label=\"test image\"", GNG_IMAGE_SIZE_WIDTH, GNG_IMAGE_SIZE_HEIGHT, GNG_IMAGE_DPI, GNG_IMAGE_RATIO, GNG_EDGE_SPLINES,  GNG_MIN_CONN_WIDTH, GNG_MAX_CONN_WIDTH, GNG_COLOR_LEN, color_list, winners, limits_of_weight, example_sensor, testing_gng, "test.gv");
	printf ("see result in \"test.gv\"\n");



	for (int i = 0; i < GNG_LIMIT_NETWORK_SIZE; i++) {
		free (testing_gng[i].weight);
		free (testing_gng[i].conn_age);
	}
	free (testing_gng);

	return 0;
}
