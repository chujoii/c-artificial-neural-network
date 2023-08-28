/*
   gng-to-dot_unit-testing.h --- header for unit-testing.c

   Copyright (C) 2019-2020 Roman V. Prikhodchenko

   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef GNG_TO_DOT_UNIT_TESTING_H
#define GNG_TO_DOT_UNIT_TESTING_H

/* adaptation coefficients for weight and local-error */
#define GNG_EPS_WINNER 0.1
#define GNG_EPS_NEIGHBOUR 0.01
#define GNG_EPS_LOCAL_ERROR 0.5
#define GNG_FACTOR_BETA_DECREASE_LOCAL_ERROR 0.1

#define GNG_LIMIT_CONN_AGE 3

/*
  big value of k-utility leave rare neurons and cause slow adaptation
  small value for fast remove too many neurons, and accordingly fast adaptation
*/
#define GNG_K_UTILITY 2.0

/* Adaptation step (add neuron in each LAMBDA_STEP to network)

   Inactived nodes may appear if lambda=very_small (high frequency
   insertion).

   For lambda=big start to learning goes very smoothly, but rough
   clusters are created */
#define GNG_LAMBDA_STEP 1L


#define GNG_LIMIT_NETWORK_SIZE 10
#define GNG_DIMENSION_OF_SENSOR 4


#define GNG_IMAGE_SIZE_WIDTH 100
#define GNG_IMAGE_SIZE_HEIGHT 80
#define GNG_IMAGE_DPI 100


#define GNG_IMAGE_RATIO "compress"

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
#define GNG_EDGE_SPLINES "true"

#define GNG_MIN_CONN_WIDTH 1
#define GNG_MAX_CONN_WIDTH 4

#endif /* GNG_TO_DOT_UNIT_TESTING_H */
