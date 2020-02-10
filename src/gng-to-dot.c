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
#include "gng-to-dot.h"


void convert_gng_conn_ages_to_simple_list (int limit_network_size, NEURON *gng, FILE *ifp)
{
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON) {
			for (int j=i; j<limit_network_size; j++) {
				if (gng[i].conn_age[j] >= INITIAL_CONNECTION_AGE) {
					/* list-to-string-dot-format */
					fprintf(ifp, "%d -- %d [label=\"%d\"];\n", i, j, gng[i].conn_age[j]);
					/* fixme: add weight="(distance between neurons)"
					   Weight of edge. A larger weight
					   encourages the layout to make the
					   edge length closer to that
					   specified by the len attribute.
					*/
				}
			}
		}
	}
}




//in-limit? limits weights
//port-position index list-of-port-positions
//number-to-group-color number group
//weights-to-string weights
//
// fixme: index_column_list list_of_port_positions list_of_groups weight_limits current_sensor_weight utilities
void convert_gng_to_string_node_attributes (int color_len, char * color_list[], int limit_network_size, NEURON *gng, FILE *ifp)
{
	/* fixme: move to function: search Umin and Umax for node diameter */
	int counter = 0;
	float Umin, Umax;
	Umin = Umax = 0.0; // fix warning
	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON) {
			if (counter == 0) { /* choise first element */
				counter ++;
				Umin = Umax = gng[i].utility_factor;
			} else { /* find extremum */
				if (Umin > gng[i].utility_factor) {Umin = gng[i].utility_factor;}
				if (Umax < gng[i].utility_factor) {Umax = gng[i].utility_factor;}
			}
		}
	}



	for (int i=0; i<limit_network_size; i++) {
		if (gng[i].active == ON) {
			fprintf (ifp, "%d [fillcolor=%s, width=%.2f];\n",
				 i,
				 color_list[gng[i].group % color_len],
				 /* utilities ("node width = diameter"):
				    D   - Dmin    U   - Umin
				    ----------- = -----------
				    Dmax - Dmin   Umax - Umin
				 */
				 (((gng[i].utility_factor - Umin) * (DMAX - DMIN)) / (Umax - Umin)) + DMIN);
		}
	}
}



void add_head (int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, FILE *ifp)
{
	fprintf (ifp, "graph ai {\n");
	fprintf (ifp, "graph [size=\"%d,%d\", dpi=%d, ratio=\"%s\"];\n",
		image_size_width, image_size_height, image_dpi, image_ratio);
	fprintf (ifp, "node [shape=circle, color=darkgreen, style=filled, penwidth=3];\n");
	fprintf (ifp, "edge [color=black, penwidth=3];\n");

	/* Adding additional space around the nodes */
	fprintf (ifp, "sep=\"+11\";\n");

	/*
	  Adding space for edge. Margin used around polygons for
	  purposes of SPLINE edge routing. Should normally be
	  strictly less than sep.
	*/
	fprintf (ifp, "esep=\"+10\";\n");

	/*
	  Controls how, and if, edges are represented.
	  True (spline): nice edges, but increase CPU load.

	  false=line (time=3.23s),
	  polyline (time=10.40s),
	  curved (time=3.25s),
	  ortho (time=3.22s),
	  true=spline (time=10.35s),
	  compound for fdp
	*/
	fprintf (ifp, "splines=%s;\n", edge_splines);

	/* Determines if and how node overlaps should be removed. */
	fprintf (ifp, "overlap=scalexy;\n");

	/*
	  When drawn, a node’s actual size is the greater of the
	  requested size and the area needed for its text label,
	  unless fixedsize=true, in which case the width and height
	  values are enforced.
	*/
	fprintf (ifp, "fixedsize=true;\n");
	fprintf (ifp, "\n");
}



void add_tail (FILE *ifp)
{
	fprintf (ifp, "}\n");
}


/* fixme: add: list_for_print_tooltip list_of_port_positions list_of_groups limits_of_weight current_sensor_weight winners */
void gng_to_dot_file (int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, int color_len, char * color_list[], int limit_network_size, NEURON *gng, char *file_name)
{
	FILE *ifp;

	ifp = fopen(file_name, "w");
	if (ifp != NULL) {
		add_head (image_size_width, image_size_height, image_dpi, image_ratio, edge_splines, ifp);
		convert_gng_to_string_node_attributes (color_len, color_list, limit_network_size, gng, ifp);
		convert_gng_conn_ages_to_simple_list (limit_network_size, gng, ifp);
		add_tail (ifp);
	}
	fclose(ifp);
}
