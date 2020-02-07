/*
   gng-to-dot.scm ---  Convert network generating from growing-neural-gas to DOT for graphviz

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
#include "growing-neural-gas.h"



void convert_gng_conn_ages_to_simple_list (int limit_network_size, NEURON *gng, FILE *ifp)
{
	for (int i=0; i<limit_network_size; i++) {
		gng[i].active = ON;
		for (int j=i; j<limit_network_size; j++) {
			if (gng[i].conn_age[j] >= INITIAL_CONNECTION_AGE) {
				fprintf(ifp, "%d -- %d\n", i, j);
			}
		}
	}
}



//list-to-string-dot-format conn-list
//in-limit? limits weights
//port-position index list-of-port-positions
//number-to-group-color number group
//convert-gng-to-string-node-attributes index-column-list list-of-port-positions list-of-groups weight-limits current-sensor-weight weights utilities
//weights-to-string weights
//add-head-tail winners body tooltip
//gng-to-dot-file list-for-print-tooltip list-of-port-positions list-of-groups limits-of-weight current-sensor-weight winners gng filename
