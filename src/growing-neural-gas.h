/* 
   growing-neural-gas.h --- header for growing-neural-gas.c
   
   Copyright (C) 2019 Roman V. Prikhodchenko
   
   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GROWING_NEURAL_GAS_H
#define GROWING_NEURAL_GAS_H

#define NOT_CONNECTED -1
#define INITIAL_CONNECTION_AGE 0



typedef struct Neuron {
	unsigned char active;
        float weight[DIMENSION_OF_SENSOR];
        int conn_age[LIMIT_NETWORK_SIZE];
        float local_error;
        float utility_factor;
} NEURON;

void initialization (NEURON *gng);
int print_neuron (NEURON neuron);

#endif /* GROWING_NEURAL_GAS_H */
