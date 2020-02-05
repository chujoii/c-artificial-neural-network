/* 
   vector.h --- header for vector.c
   
   Copyright (C) 2019 Roman V. Prikhodchenko
   
   Author: Roman V. Prikhodchenko <chujoii@gmail.com>
*/

#ifndef VECTOR_H
#define VECTOR_H

#define square(x) (x)*(x)

float euclidean_distance (float a, float b);
float euclidean_distance_vector (float *a, float *b, int size);
float angle_distance (float a, float b);
	
#endif /* VECTOR_H */
