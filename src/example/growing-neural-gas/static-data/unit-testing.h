/*
   unit-testing.h --- header for unit-testing.c

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef UNIT_TESTING_H
#define UNIT_TESTING_H

/* adaptation coefficients for weight and local-error */
#define EPS_WINNER 0.1
#define EPS_NEIGHBOUR 0.01
#define EPS_LOCAL_ERROR 0.5
#define FACTOR_BETA_DECREASE_LOCAL_ERROR 0.1

#define LIMIT_CONN_AGE 3

/*
  big value of k-utility leave rare neurons and cause slow adaptation
  small value for fast remove too many neurons, and accordingly fast adaptation
*/
#define K_UTILITY 2.0

/* Adaptation step (add neuron in each LAMBDA_STEP to network)

   Inactived nodes may appear if lambda=very_small (high frequency
   insertion).
   
   For lambda=big start to learning goes very smoothly, but rough
   clusters are created */
#define LAMBDA_STEP 1L


#define LIMIT_NETWORK_SIZE 10
#define DIMENSION_OF_SENSOR 4


#endif /* UNIT_TESTING_H */
