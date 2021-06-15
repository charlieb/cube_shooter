#include "shapes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void stransform(shape *s, mat m) {
	vec res;
	for(int i = 0; i < s->npts; i++) {
		mvmul(m, s->pts[i], res);
		memcpy(s->pts[i], res, sizeof(vec));
	}
}

void print_shape(shape *s) {
	printf("Shape: npts: %i, nlines: %i\n", s->npts, s->proto->nlines);
	for(int i = 0; i < s->npts; i++)
		printf("%4f, %4f, %4f, %4f\n", 
				s->pts[i][0],
				s->pts[i][1],
				s->pts[i][2],
				s->pts[i][3]);
	for(int i = 0; i < s->proto->nlines; i++)
		printf("%i, %i\n", s->proto->lines[i*2], s->proto->lines[i*2+1]);


	for(int i = 0; i < s->proto->nlines; i++)
		printf("%4f, %4f, %4f, %4f -> %4f, %4f, %4f, %4f\n", 
				s->pts[s->proto->lines[i*2]][0],
				s->pts[s->proto->lines[i*2]][1],
				s->pts[s->proto->lines[i*2]][2],
				s->pts[s->proto->lines[i*2]][3],
				s->pts[s->proto->lines[i*2+1]][0],
				s->pts[s->proto->lines[i*2+1]][1],
				s->pts[s->proto->lines[i*2+1]][2],
				s->pts[s->proto->lines[i*2+1]][3]
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
/*
typedef struct sfixture {
	mat m;
	matstack ms;
	shape *s;
	int nkids;
	int nkids_alloced;
	struct sfixture **kids;
} fixture;
*/

void frender(fixture *fix) {
	const int todo_block_size = 10;
	int todo_size = todo_block_size;
	/* TODO maybe static this to avoid allocations */
	fixture **todo = malloc(todo_size * sizeof(fixture*));
	int head = 0;

	mscalc(&fix->ms, fix->m);
	fixture *f = fix, *k;
	mat tmp;

	while(head >= 0) {
		/*make sure there's enough space for the kids*/
		while(head + f->nkids >= todo_size) {
			todo_size += todo_block_size;
			todo = realloc(todo, todo_size * sizeof(fixture*));
		}
		/*add the kids and calc them at the same time*/
		for(int i = 0; i < f->nkids; i++) {
			k = f->kids[i];
			mscalc(&k->ms, k->m);
			mmul(f->m, k->m, tmp); // may need to reverse this
			memcpy(k->m, tmp, sizeof(mat));

			todo[head+i] = k;
		}
		head += f->nkids -1; // point to the last child
		/*pop the last child and process its children in the next iteration*/
		f = todo[head--];
	}
	free(todo);
}

void fixture_show(fixture *f) {
}
