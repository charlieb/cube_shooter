#ifndef _SHAPES_
#define _SHAPES_

#include "matrix.h"

typedef struct {
	int npts;
	vec *orig_pts; // TODO pull this out into a seperate protoype shape
	vec *pts;
	int nlines;
	int *lines;
	matstack ms;
} shape;

void print_shape(shape *s);
void stransform(shape *s, mat m);

#endif
