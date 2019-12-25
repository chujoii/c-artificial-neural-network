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

/* Use  for debug print, or #f for silent */
//#define DEBUG
// uncomment previous string for debug=ON
#ifdef DEBUG
 #define DEBUG_PRINT(x)  fprintf (stderr, x)
#else
 #define DEBUG_PRINT(x)
#endif

/* adaptation coefficients for weight and local-error */
#define EPS_WINNER 0.1
#define EPS_NEIGHBOUR 0.01
#define EPS_LOCAL_ERROR 0.5
#define FACTOR_BETA_DECREASE_LOCAL_ERROR 0.9

#define LIMIT_CONN_AGE 3

/* Adaptation step (add neuron in each LAMBDA_STEP to network)

   Inactived nodes may appear if lambda=very_small (high frequency
   insertion).
   
   For lambda=big start to learning goes very smoothly, but rough
   clusters are created */
#define LAMBDA_STEP 1

#define LIMIT_NETWORK_SIZE 10

#define DIMENSION_OF_SENSOR 4

#define LIMIT_NETWORK_SIZE 10

#include "../../../growing-neural-gas.h"

int main ()
{
	return 0;
}
