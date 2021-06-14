#ifndef _SHAPES_
#define _SHAPES_

#include "matrix.h"


typedef struct {
	int npts;
	vec *pts;
	int nlines;
	int *lines;
} shape;

#define FIXTURE_CHUNK 5
struct sfixture;
typedef struct sfixture {
	matstack ms;
	shape *s;
	int nkids;
	int nkids_alloced;
	struct sfixture **kids;
} fixture;

void print_shape(shape *s);
void stransform(shape *s, mat m);

void fpush(fixture *f, fixture *kid);
void fixture_show(fixture *f);

#endif
