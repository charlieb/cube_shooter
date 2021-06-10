#ifndef _SHAPES_
#define _SHAPES_

#include "matrix.h"

typedef struct {
	int npts;
	vec *pts;
	int nlines;
	int *lines;
} shape;

void print_shape(shape *s);

#endif
