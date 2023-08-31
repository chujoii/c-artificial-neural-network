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
#include <string.h> // only for for strcpy
#include <stdlib.h> // only for abs
#include <math.h> // check for NaN
#include <float.h> // similar to limits.h for int, ...
#include "growing-neural-gas.h"
#include "gng-to-dot.h"


void convert_gng_conn_ages_to_simple_list (int min_conn_width, int max_conn_width, GNG_NEURON *gng, FILE *ifp)
{
	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON) {
			for (int j = i; j < gng->limit_network_size; j++) {
				if (gng[i].conn_age[j] >= GNG_INITIAL_CONNECTION_AGE) {
					/* list-to-string-dot-format */
					fprintf(ifp, "%d -- %d [label=\"%d\", penwidth=%.1f];\n", i, j, gng[i].conn_age[j],
						max_conn_width + gng[i].conn_age[j]*(min_conn_width - max_conn_width - 1)/ gng->limit_conn_age);
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




/* return 0 if weights in limit
   return 1 if weights out of limit
 */
int in_limit (int dimension_of_sensor, float limits[][2], float *weights)
{
	for (int i = 0; i < dimension_of_sensor; i++) {
		if ((weights[i] < limits[i][GNG_LO]) || (weights[i] > limits[i][GNG_HI]))
			return 1;
	}
	return 0;
}



void fill_limits (int dimension_of_sensor, float limits_of_weight[][2])
{
	for(int i = 0; i < dimension_of_sensor; i++) {
		limits_of_weight[i][GNG_LO] = -FLT_MAX; // see float.h
		limits_of_weight[i][GNG_HI] = FLT_MAX; // see float.h
	}
	/* fixme: all sensor data does not have limits except voltage */
	/* fixme:     Ua=0                      Ub=1                      Uc=3        normal_value  +-   deviation*/
	limits_of_weight[0][GNG_LO] = limits_of_weight[1][GNG_LO] = limits_of_weight[3][GNG_LO] =      10       -    5;
	limits_of_weight[0][GNG_HI] = limits_of_weight[1][GNG_HI] = limits_of_weight[3][GNG_HI] =      10       +    5;
}



// fixme: index_column_list list_of_port_positions list_of_groups weight_limits current_sensor_weight utilities
void convert_gng_to_string_node_attributes (int color_len, char * color_list[], float limits_of_weight[][2], GNG_NEURON *gng, FILE *ifp)
{
	float Umin, Umax;
	extremum_utility_factor (&Umin, &Umax, gng);
	if ((Umax - Umin) < 0.001) { // fixme: NaN
		Umax += 0.001;
	}

	char port[4]; // ":se\0" min_size=4
	char *port_positions[] = {"n", "ne", "e", "se", "s", "sw", "w", "nw", "c", "_"};
	int num_of_port_positions = 8; // from "n"-to-"nw" (not include "_")
	int group_checked [gng->limit_network_size];
	for (int i = 0; i < gng->limit_network_size; i++) {
		group_checked[i] = 0;
	}

	for (int i = 0; i < gng->limit_network_size; i++) {
		if (gng[i].active == GNG_ON) {
			if (group_checked[gng[i].group] == 0) { // if not used, set position pinned
				group_checked[gng[i].group] = 1;
				sprintf (port, ":%s", port_positions[gng[i].group % num_of_port_positions]);
			} else { // position already pinned
				strcpy (port, "");
			}
			fprintf (ifp, "%d%s [%sfillcolor=%s, width=%.2f];\n",
				 i, port,
				 // green color for border already set in node section (node [shape=circle, color=green,...)
				 (in_limit(gng->dimension_of_sensor, limits_of_weight, gng[i].weight) == 0) ? "" : "color=red, ",
				 color_list[abs(gng[i].group) % color_len],
				 /* utilities ("node width = diameter"):
				    D   - Dmin    U   - Umin
				    ----------- = -----------
				    Dmax - Dmin   Umax - Umin
				 */
				 //  if (isnan(a)) ... OR  if (isnoraml(a)) ... , fixme: need check it at calculation algorithm
				 (isnormal(gng[i].utility_factor)) ? ((((gng[i].utility_factor - Umin) * (GNG_DMAX - GNG_DMIN)) / (Umax - Umin)) + GNG_DMIN) : GNG_DMAX);
		}
	}
}



void add_head (char *img_caption, int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, FILE *ifp)
{
	fprintf (ifp, "graph ai {\n");
	fprintf (ifp, "graph [size=\"%d,%d\", dpi=%d, ratio=\"%s\", %s];\n",
		 image_size_width, image_size_height, image_dpi, image_ratio, img_caption);
	fprintf (ifp, "node [shape=circle, color=green, style=filled, penwidth=3];\n");
	fprintf (ifp, "edge [color=black];\n");

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


/* fixme: add: list_for_print_tooltip list_of_groups */
void gng_to_dot_file (char *img_caption, int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, int min_conn_width, int max_conn_width, int color_len, char * color_list[], int *winners, float limits_of_weight[][2], float *current_sensor_weight, GNG_NEURON *gng, char *file_name)
{
	FILE *ifp;

	ifp = fopen(file_name, "w");
	if (ifp != NULL) {
		add_head (img_caption, image_size_width, image_size_height, image_dpi, image_ratio, edge_splines, ifp);

		fprintf(ifp, "c -- %d [style=dashed];\n", winners[0]);
		fprintf(ifp, "c -- %d [style=dotted];\n", winners[1]);
		fprintf(ifp, "c [tooltip=\"replace by measured value\", shape=box, color=black, fillcolor=%s, fontcolor=white];\n",
			(in_limit(gng->dimension_of_sensor, limits_of_weight, current_sensor_weight) == 0) ? "green" : "red");

		convert_gng_to_string_node_attributes (color_len, color_list, limits_of_weight, gng, ifp);
		convert_gng_conn_ages_to_simple_list (min_conn_width, max_conn_width, gng, ifp);
		add_tail (ifp);
	}
	fclose(ifp);
}
