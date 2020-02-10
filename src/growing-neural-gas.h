/*
   growing-neural-gas.h --- header for growing-neural-gas.c

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GROWING_NEURAL_GAS_H
#define GROWING_NEURAL_GAS_H

#define NOT_CONNECTED -1
#define INITIAL_CONNECTION_AGE 0

#define NOT_IN_ANY_GROUPS -1

#define OFF 0
#define ON 1

enum mixed_space_distance_variants { EUCLIDEAN, ANGLE };

typedef struct Neuron {
	unsigned char active;
        float *weight; // DIMENSION_OF_SENSOR * sizeof (* (gng[i].weight))
        int *conn_age; // LIMIT_NETWORK_SIZE * sizeof (* (gng[i].conn_age))
        float local_error;
        float utility_factor;
	int group; // only for visualization
} NEURON;

typedef struct LocalError {
	int index_in_gng;
        float local_error;
} LOCALERROR;

void initialization (int dimension_of_sensor, int limit_network_size, NEURON *gng);
int print_neuron (int dimension_of_sensor, int limit_network_size, NEURON neuron);
// fixme: print-gng-as-list (gng)
int add_neuron (int dimension_of_sensor, int limit_network_size, NEURON *gng);
void find_and_del_neuron_with_min_utility_factor (float k, int limit_network_size, NEURON *gng);
int index_of_minimum_utility_factor (int limit_network_size, NEURON *gng);
int length_gng (int limit_network_size, NEURON *gng);
float value_of_median_local_error (int limit_network_size, NEURON *gng);
void reconnect (int limit_network_size, NEURON *gng);
void update_neuron_weight_vector(int neuron_a, float step, float *sensor, int dimension_of_sensor, NEURON *gng);
void update_neighbours_weights (int neuron_a, float eps_step, float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng);
void update_neuron_conn_age (int neuron_a, int neuron_b, int step, int limit_network_size, NEURON *gng);
void set_neuron_conn_age (int neuron_a, int neuron_b, int conn_age, int limit_network_size, NEURON *gng);
void inc_neighbours_conn_age (int neuron_a, int limit_network_size, NEURON *gng);
void remove_old_conn_age (int limit_conn_age, int limit_network_size, NEURON *gng);
void update_neuron_local_error (int neuron_a, float step, NEURON *gng);
void update_neuron_utility_factor (int neuron_a, float step, NEURON *gng);
void decrease_all_neuron_local_errors_and_utility_factor (float factor_beta, int limit_network_size, NEURON *gng);
void calculate_distance_weight_sensor (float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng, float *distance);
void calculate_distance_in_mixed_space_weight_sensor (int *mixed_space, float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng, float *return_distance);
void find_index_of_two_minimal (float *in_arr, int in_size, int *out_indexes);
int find_neuron_index_with_max_local_error (int limit_network_size, NEURON *gng);
int find_neighbours_index_with_max_local_error (int index_max_local_error, int limit_network_size, NEURON *gng);
void adaptive_step_create_new_neuron (float eps_local_error, int dimension_of_sensor, int limit_network_size, NEURON *gng);
void growing_neural_gas (int epoch, float eps_winner, float eps_neighbour, float eps_local_error, float factor_beta_decrease_local_error, int limit_conn_age, float k_utility, int lambda_step, int *mixed_space, float *sensor, int dimension_of_sensor, int limit_network_size, NEURON *gng);
void extract_groups_from_conn_ages (int limit_network_size, NEURON *gng);
void ungroup (int neuron_a, int limit_network_size, NEURON *gng);
int write_gng_to_file (char *file_name, int dimension_of_sensor, int limit_network_size, NEURON *gng);
int read_gng_from_file (char *file_name, int dimension_of_sensor, int limit_network_size, NEURON *gng);

#endif /* GROWING_NEURAL_GAS_H */
