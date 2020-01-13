/* 
   growing-neural-gas.h --- header for growing-neural-gas.c
   
   Copyright (C) 2019 Roman V. Prikhodchenko
   
   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GROWING_NEURAL_GAS_H
#define GROWING_NEURAL_GAS_H

#define NOT_CONNECTED -1
#define INITIAL_CONNECTION_AGE 0

#define OFF 0
#define ON 1

enum mixed_space_distance_variants { EUCLIDEAN, ANGLE };

typedef struct Neuron {
	unsigned char active;
        float weight[DIMENSION_OF_SENSOR];
        int conn_age[LIMIT_NETWORK_SIZE];
        float local_error;
        float utility_factor;
} NEURON;

void initialization (NEURON *gng);
int print_neuron (NEURON neuron);
// fixme: print-gng-as-list (gng)
int add_neuron (NEURON *gng);
// fixme: find-and-del-neuron-with-min-utility-factor (k gng)
void update_neuron_weight_vector(int neuron_a, float step, float *sensor, NEURON *gng);
// fixme: update-neighbours-weights (function list-neighbour eps-step gng)
void update_neuron_conn_age (int neuron_a, int neuron_b, int step, NEURON *gng);
void disconnect_neuron (int neuron_a, int neuron_b, NEURON *gng);
// fixme: inc-neighbours-conn-age (a gng)
// fixme: remove-old-conn-age (limit-conn-age gng)
void update_neuron_local_error (int neuron_a, float step, NEURON *gng);
// fixme: update-neuron-utility-factor (a function step gng)
// fixme: decrease-all-neuron-local-errors-and-utility-factor (factor-beta gng)
void calculate_distance_weight_sensor (float *sensor, NEURON *gng, float *distance);
void calculate_distance_in_mixed_space_weight_sensor (int *mixed_space, float *sensor, NEURON *gng, float *return_distance);
void find_index_of_two_minimal (float *in_arr, int in_size, int *out_indexes);
// fixme: find-neuron-index-with-max-local-error (gng)
// fixme: find-neighbours-index-with-max-local-error (index-max-local-error gng)
// fixme: adaptive-step-create-new-neuron (gng)
// fixme: growing-neural-gas epoch sensor (gng)

#endif /* GROWING_NEURAL_GAS_H */
