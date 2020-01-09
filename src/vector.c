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

#define square(x) (x)*(x)


float euclidean_distance_vector (float *a, float *b, int size)
{
	float sum = 0.0;

	for (int i=0; i<size; i++) {
		sum += square (a[i] - b[i]);
	}

	return sqrt (sum);
}
