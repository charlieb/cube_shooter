#include "shapes.h"

#include <stdlib.h>

shape *alloc_shape(int npts, int nlines) {
	shape *s = malloc(sizeof(shape));
	s->pts = malloc(npts * sizeof(vec));
	s->lines = malloc(nlines * sizeof(int));
	return s;
}

shape *alloc_shapes(int nshapes, int npts, int nlines) {
	shape *s = malloc(nshapes * sizeof(shape));
	vec *pts = malloc(nshapes * npts * sizeof(vec));
	int *lines = malloc(nshapes * nlines * sizeof(int));

	for(int i = 0; i < nshapes; i++) {
		s[i].pts = pts + i * npts;
		s[i].lines = lines + i * nlines;
	}

	return s;
}
