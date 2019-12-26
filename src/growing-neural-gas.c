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

#include "growing-neural-gas.h"

void print_neuron (int num_neuron, NEURON *gng)
{
	printf("w:");
	for (int i=0; i<DIMENSION_OF_SENSOR; i++) {
		printf(" %7.1f", gng[num_neuron].weight[i]);
	}
	
	printf("\ta:");
	for (int i=0; i<LIMIT_NETWORK_SIZE; i++) {
		printf(" %d", gng[num_neuron].conn_age[i]);
	}
	
	printf("\te: %5.1f\t", gng[num_neuron].local_error);
	printf("u: %5.1f\n", gng[num_neuron].utility_factor);
}
