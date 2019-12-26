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
#include "../../../growing-neural-gas.c"

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
	NEURON testing_gng[LIMIT_NETWORK_SIZE];
	printf("empty network: (%d elements)\n", LIMIT_NETWORK_SIZE);
	initialization (testing_gng);
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}

	printf("\nadd neuron: number=%d\n", add_neuron(testing_gng));
	printf("add neuron: number=%d\n", add_neuron(testing_gng));

	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		print_neuron (testing_gng[i]);
	}

	return 0;
}
