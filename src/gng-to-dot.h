/*
   gng-to-dot.h --- header for gng-to-dot.c

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GNG_TO_DOT_H
#define GNG_TO_DOT_H

// diameter of node
#define DMIN 0.1
#define DMAX 1.0

#define LO 0
#define HI 1

void convert_gng_conn_ages_to_simple_list (int limit_network_size, NEURON *gng, FILE *ifp);
void convert_gng_to_string_node_attributes (int color_len, char * color_list[], float limits_of_weight[][2], int dimension_of_sensor, int limit_network_size, NEURON *gng, FILE *ifp);
void add_head (char *img_caption, int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, FILE *ifp);
void add_tail (FILE *ifp);
void gng_to_dot_file (char *img_caption, int image_size_width, int image_size_height, int image_dpi, char *image_ratio, char *edge_splines, int color_len, char * color_list[], int *winners, float limits_of_weight[][2], float *current_sensor_weight, int dimension_of_sensor, int limit_network_size, NEURON *gng, char *file_name);

#endif /* GNG_TO_DOT_H */
