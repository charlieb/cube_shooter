#ifndef _SHAPES_
#define _SHAPES_

#include "matrix.h"

struct fixture;

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
	struct fixture *fixture;
} shape;

#define FIXTURE_CHUNK 5
typedef struct fixture {
	mat m;
	matstack ms;
	shape *s;
	struct fixture *parent;
	int nkids;
	int nkids_alloced;
	struct fixture **kids;
} fixture;

void print_shape(shape *s);
void stransform(shape *s, mat m);

void fpush(fixture *f, fixture *kid);
void fmspush(fixture *f, mat m);
void frender(fixture *f, int *nshapes, shape ***shape);

void fixture_show(fixture *f);

#endif
