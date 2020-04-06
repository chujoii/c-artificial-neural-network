/*
   gng-to-dot_unit-testing.h --- header for unit-testing.c

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GNG_TO_DOT_UNIT_TESTING_H
#define GNG_TO_DOT_UNIT_TESTING_H

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


#define IMAGE_SIZE_WIDTH 100
#define IMAGE_SIZE_HEIGHT 80
#define IMAGE_DPI 100


#define IMAGE_RATIO "compress"

/*
  Controls how, and if, edges are represented.
  True (spline): nice edges, but increase CPU load.

  false=line (time=3.23s),
  polyline (time=10.40s),
  curved (time=3.25s),
  ortho (time=3.22s),
  true=spline (time=10.35s),
  compound (for fdp only time=?)
*/
#define EDGE_SPLINES "true"

#define MIN_CONN_WIDTH 1
#define MAX_CONN_WIDTH 4

#endif /* GNG_TO_DOT_UNIT_TESTING_H */
