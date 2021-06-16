#ifndef _SHAPES_
#define _SHAPES_

#include "matrix.h"

typedef struct {
	int npts;
	vec *pts;
	int nlines;
	int *lines;
} proto_shape;

typedef struct {
	proto_shape *proto;
	int npts;
	vec *pts;
} shape;

#define FIXTURE_CHUNK 5
struct sfixture;
typedef struct sfixture {
	mat m;
	matstack ms;
	shape *s;
	struct sfixture *parent;
	int nkids;
	int nkids_alloced;
	struct sfixture **kids;
} fixture;

void print_shape(shape *s);
void stransform(shape *s, mat m);

void fpush(fixture *f, fixture *kid);
void frender(fixture *f);

void fixture_show(fixture *f);

#endif
