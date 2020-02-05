/* 
   vector.c --- Vector functions for mathematics and physics
   
   Copyright (C) 2020 Roman V. Prikhodchenko
   
   Author: Roman V. Prikhodchenko <chujoii@gmail.com>

   This file is part of c-artificial-neural-network.
   
   c-artificial-neural-network is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   c-artificial-neural-network is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with c-artificial-neural-network.  If not, see <http://www.gnu.org/licenses/>.
   
*/

#include <math.h>

#include "vector.h"




// for distance need add: (sqrt (+ (euclidean-distance a b) (euclidean-distance c d) (euclidean-distance e f) ...))
float euclidean_distance (float a, float b)
{
	return square (a - b);
}



float euclidean_distance_vector (float *a, float *b, int size)
{
	float sum = 0.0;

	for (int i=0; i<size; i++) {
		sum += square (a[i] - b[i]);
	}

	return sqrt (sum);
}



float constrain_angle (float angle, float max_angle_unit)
{
	float a = fmod (angle, max_angle_unit); // remainder
	return ((a < 0.0) ? (a + max_angle_unit) : a);
}



float min_angle_between_two_angles (float a, float b, float max_angle_unit)
{
	float x = fabs (constrain_angle (a, max_angle_unit) - // no need constranin for x
			constrain_angle (b, max_angle_unit));
	return fmin (x, (max_angle_unit - x));
}



// for distance need add: (sqrt (+ (angle-distance a b) (angle-distance c d) (angle-distance e f) ...))
float angle_distance (float a, float b)
{
	return square (min_angle_between_two_angles (a, b, 360.0));
}

