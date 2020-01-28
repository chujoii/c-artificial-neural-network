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

typedef struct LocalError {
	int index_in_gng;
        float local_error;
} LOCALERROR;

void initialization (NEURON *gng);
int print_neuron (NEURON neuron);
// fixme: print-gng-as-list (gng)
int add_neuron (NEURON *gng);
void find_and_del_neuron_with_min_utility_factor (int k, NEURON *gng);
int index_of_minimum_utility_factor (NEURON *gng);
int length_gng (NEURON *gng);
float value_of_median_local_error (NEURON *gng);
void reconnect (NEURON *gng);
void update_neuron_weight_vector(int neuron_a, float step, float *sensor, NEURON *gng);
void update_neighbours_weights (int neuron_a, float eps_step, float *sensor, NEURON *gng);
void update_neuron_conn_age (int neuron_a, int neuron_b, int step, NEURON *gng);
void set_neuron_conn_age (int neuron_a, int neuron_b, int conn_age, NEURON *gng);
void inc_neighbours_conn_age (int neuron_a, NEURON *gng);
void remove_old_conn_age (int limit_conn_age, NEURON *gng);
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
