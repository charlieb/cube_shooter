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

void srender(shape *s) {
	for(int i = 0; i < s->proto->npts; i++)
		mvmul(s->fixture->m, s->proto->pts[i], s->pts[i]);
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
	kid->parent = f;

	if(f->nkids == f->nkids_alloced) {
		f->nkids_alloced += FIXTURE_CHUNK;
		f->kids = realloc(f->kids, f->nkids_alloced * sizeof(fixture*));
	}
	f->kids[f->nkids] = kid;
	f->nkids++;
}

void fmspush(fixture *f, mat m) {
	mspush(&f->ms, m);
}
/*
typedef struct sfixture {
	mat m;
	matstack ms;
	shape *s;
	struct sfixture *parent;
	int nkids;
	int nkids_alloced;
	struct sfixture **kids;
} fixture;
*/

/* gives back a list where items earlier in the list are 
 * gaurenteed to be closer to the root node of the tree.
 * This allows us to recalculate all the fixtures in order
 * and ensure that parents are calculated before the 
 * children that depend on them.
 * */
static void fcollect(fixture *fix, int *nfixtures, fixture ***list) {
	const int block_size = 10;
	int todo_size = block_size;
	/* TODO maybe static this to avoid allocations */
	fixture **todo = malloc(todo_size * sizeof(fixture*));
	int head = 0;

	int list_size = block_size;
	if(*list == NULL) // alloc a new fixture list
		*list = malloc(list_size * sizeof(fixture*));
	 else //reuse a previously allocated list
		list_size = *nfixtures;
	*nfixtures = 0;

	(*list)[(*nfixtures)++] = fix;
	todo[head] = fix;

	fixture *f = fix, *k;
	while(head >= 0) {
		f = todo[head];
		/*make sure there's enough space for the kids in todo stack*/
		while(head + f->nkids >= todo_size) {
			todo_size += block_size;
			todo = realloc(todo, todo_size * sizeof(fixture*));
		}
		/*make sure there's enough space for the kids in the output list*/
		while((*nfixtures) + f->nkids >= list_size) {
			list_size += block_size;
			*list = realloc(*list, list_size * sizeof(fixture*));
		}
		/*add the kids to todo and the output list*/
		for(int i = 0; i < f->nkids; i++) {
			k = f->kids[i];
			//printf("%p -> %p\n", f, k); fflush(NULL);
			todo[head++] = k;
			(*list)[(*nfixtures)++] = k;
		}
		head -= 1; // point to the last child
		/*pop the last child and process its children in the next iteration*/
	}
	free(todo);
}

void frender(fixture *fix, int *nshapes, shape ***shapes) {
	static fixture **fs = NULL;
	static int nfs = 0;
	fcollect(fix, &nfs, &fs);

	*shapes = realloc(*shapes, nfs * sizeof(shape*));
	*nshapes = 0;

	mat tmp;
	//for(int i = 0; i < nfs; i++) printf("%p\n", fs[i]);
	for(int i = 0; i < nfs; i++) {
		mscalc(&fs[i]->ms, tmp);
		if(fs[i]->parent)
			mmul(fs[i]->parent->m, tmp, fs[i]->m); // may need to reverse this
		else
			memcpy(fs[i]->m, tmp, sizeof(mat));

		if(fs[i]->s) {
			srender(fs[i]->s);
			(*shapes)[(*nshapes)++] = fs[i]->s;
		}
	}
	/*
	for(int i = 0; i < nfs; i++) {
		//msprint(&fs[i]->ms);
		printf("%i ---------------- %i\n", i, i);
		mprint(fs[i]->m);
	}
	*/
}

void fixture_show(fixture *f) {
}
