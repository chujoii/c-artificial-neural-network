/*
   gng-to-dot.h --- header for gng-to-dot.c

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GNG_TO_DOT_H
#define GNG_TO_DOT_H

void convert_gng_conn_ages_to_simple_list (int limit_network_size, NEURON *gng, FILE *ifp);
//list-to-string-dot-format conn-list
//in-limit? limits weights
//port-position index list-of-port-positions
//number-to-group-color number group
//convert-gng-to-string-node-attributes index-column-list list-of-port-positions list-of-groups weight-limits current-sensor-weight weights utilities
//weights-to-string weights
void add_head (int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, FILE *ifp);
void add_tail (FILE *ifp);
void gng_to_dot_file (int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, int limit_network_size, NEURON *gng, char *file_name);

#endif /* GNG_TO_DOT_H */
