/*
   growing-neural-gas.h --- header for growing-neural-gas.c

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GROWING_NEURAL_GAS_H
#define GROWING_NEURAL_GAS_H

#define GNG_NOT_CONNECTED -1
#define GNG_INITIAL_CONNECTION_AGE 0

#define GNG_NOT_IN_ANY_GROUPS -1
#define GNG_SEARCH_GROUPS -2

#define GNG_OFF 0
#define GNG_ON 1

enum mixed_space_distance_variants { GNG_EUCLIDEAN, GNG_ANGLE };

typedef struct gng_Neuron {
	unsigned char active;
        float *weight; // DIMENSION_OF_SENSOR * sizeof (* (gng[i].weight))
        int *conn_age; // LIMIT_NETWORK_SIZE * sizeof (* (gng[i].conn_age))
        float local_error;
        float utility_factor;
	int group; // only for visualization


	/* adaptation coefficients for weight and local-error */
	float eps_winner;
	float eps_neighbour;
	float eps_local_error;
	float factor_beta_decrease_local_error;

	float limit_conn_age;

	/*
	  big value of k-utility leave rare neurons and cause slow adaptation
	  small value for fast remove too many neurons, and accordingly fast adaptation
	*/
	float k_utility;

	/* Adaptation step (add neuron in each LAMBDA_STEP to network)

	   Inactived nodes may appear if lambda=very_small (high frequency
	   insertion).

	   For lambda=big start to learning goes very smoothly, but rough
	   clusters are created
	*/
	unsigned long int lambda_step;


	int limit_network_size;
	int dimension_of_sensor;
} GNG_NEURON;

typedef struct gng_LocalError {
	int index_in_gng;
        float local_error;
} GNG_LOCALERROR;

void initialization (float eps_winner,
		     float eps_neighbour,
		     float eps_local_error,
		     float factor_beta_decrease_local_error,
		     float limit_conn_age,
		     float k_utility,
		     unsigned long int lambda_step,
		     int limit_network_size,
		     int dimension_of_sensor,
		     GNG_NEURON *gng);
int print_gng_neuron (int dimension_of_sensor, int limit_network_size, GNG_NEURON neuron);
// fixme: print-gng-as-list (gng)
int add_gng_neuron (GNG_NEURON *gng);
void find_and_del_neuron_with_min_utility_factor (GNG_NEURON *gng);
void extremum_utility_factor (float *Umin, float *Umax, GNG_NEURON *gng);
int index_of_minimum_utility_factor (GNG_NEURON *gng);
int length_gng (GNG_NEURON *gng);
float value_of_median_local_error (GNG_NEURON *gng);
void reconnect (GNG_NEURON *gng);
void update_neuron_weight_vector(int neuron_a, float step, float *sensor, GNG_NEURON *gng);
void update_neighbours_weights (int neuron_a, float eps_step, float *sensor, GNG_NEURON *gng);
void update_neuron_conn_age (int neuron_a, int neuron_b, int step, GNG_NEURON *gng);
void set_neuron_conn_age (int neuron_a, int neuron_b, int conn_age, GNG_NEURON *gng);
void inc_neighbours_conn_age (int neuron_a, GNG_NEURON *gng);
void remove_old_conn_age (GNG_NEURON *gng);
void update_neuron_local_error (int neuron_a, float step, GNG_NEURON *gng);
void update_neuron_utility_factor (int neuron_a, float step, GNG_NEURON *gng);
void decrease_all_neuron_local_errors_and_utility_factor (GNG_NEURON *gng);
void calculate_distance_weight_sensor (float *sensor, GNG_NEURON *gng, float *distance);
void calculate_distance_in_mixed_space_weight_sensor (int *mixed_space, float *sensor, GNG_NEURON *gng, float *return_distance);
void find_index_of_two_minimal (float *in_arr, int in_size, int *out_indexes);
int find_neuron_index_with_max_local_error (GNG_NEURON *gng);
int find_neighbours_index_with_max_local_error (int index_max_local_error, GNG_NEURON *gng);
void adaptive_step_create_new_neuron (GNG_NEURON *gng);
void growing_neural_gas (unsigned long int epoch, int winners_for_return_from_function[2], int *mixed_space, float *sensor, GNG_NEURON *gng);
void extract_groups_from_conn_ages (GNG_NEURON *gng);
void ungroup (int neuron_a, GNG_NEURON *gng);
int write_gng_to_file (char *file_name, GNG_NEURON *gng);
int read_gng_from_file (char *file_name, GNG_NEURON *gng);

#endif /* GROWING_NEURAL_GAS_H */
