#include "shapes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

shape *alloc_shape(int npts, int nlines) {
	shape *s = malloc(sizeof(shape));
	s->npts = npts;
	s->pts = malloc(npts * sizeof(vec));
	s->nlines = nlines;
	s->lines = malloc(nlines * sizeof(int));
	return s;
}

shape *alloc_shapes(int nshapes, int npts, int nlines) {
	shape *s = malloc(nshapes * sizeof(shape));
	vec *pts = malloc(nshapes * npts * sizeof(vec));
	int *lines = malloc(nshapes * nlines * sizeof(int));

	for(int i = 0; i < nshapes; i++) {
		s[i].npts = npts;
		s[i].pts = pts + i * npts;
		s[i].nlines = nlines;
		s[i].lines = lines + i * nlines;
	}

	return s;
}

void stransform(shape *s, mat m) {
	vec res;
	for(int i = 0; i < s->npts; i++) {
		mvmul(m, s->pts[i], res);
		memcpy(s->pts[i], res, sizeof(vec));
	}
}

void print_shape(shape *s) {
	printf("Shape: npts: %i, nlines: %i\n", s->npts, s->nlines);
	for(int i = 0; i < s->npts; i++)
		printf("%4f, %4f, %4f, %4f\n", 
				s->pts[i][0],
				s->pts[i][1],
				s->pts[i][2],
				s->pts[i][3]);
	for(int i = 0; i < s->nlines; i++)
		printf("%i, %i\n", s->lines[i*2], s->lines[i*2+1]);


	for(int i = 0; i < s->nlines; i++)
		printf("%4f, %4f, %4f, %4f -> %4f, %4f, %4f, %4f\n", 
				s->pts[s->lines[i*2]][0],
				s->pts[s->lines[i*2]][1],
				s->pts[s->lines[i*2]][2],
				s->pts[s->lines[i*2]][3],
				s->pts[s->lines[i*2+1]][0],
				s->pts[s->lines[i*2+1]][1],
				s->pts[s->lines[i*2+1]][2],
				s->pts[s->lines[i*2+1]][3]
				);
}

void fpush(fixture *f, fixture *kid) {
	if(f->nkids == f->nkids_alloced) {
		f->nkids_alloced += FIXTURE_CHUNK;
		f->kids = realloc(f->kids, f->nkids_alloced * sizeof(fixture*));
	}
	f->kids[f->nkids] = kid;
	f->nkids++;
}

struct fixture_render {
	vec *pts;
	int *lines;
};

void falloc_render(fixture *f, struct fixture_render **fr) {

}

void fixture_show(fixture *f) {
}
