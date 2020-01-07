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

typedef struct Neuron {
	unsigned char active;
        float weight[DIMENSION_OF_SENSOR];
        int conn_age[LIMIT_NETWORK_SIZE];
        float local_error;
        float utility_factor;
} NEURON;

void initialization (NEURON *gng);
int print_neuron (NEURON neuron);
int add_neuron (NEURON *gng);
void inc_neuron_conn_age (int neuron_a, int neuron_b, int step, NEURON *gng);
void disconnect_neuron (int neuron_a, int neuron_b, NEURON *gng);
//void update_neuron_weight_vector(float step, float *sensor, NEURON neuron);
void update_neuron_weight_vector(int neuron_a, float step, float *sensor, NEURON *gng);

#endif /* GROWING_NEURAL_GAS_H */
