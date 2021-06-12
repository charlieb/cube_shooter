#ifndef _MATRIX_
#define _MATRIX_

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

typedef float mat [4][4];
typedef float vec [4];

void mprint(mat m);
void vprint(vec v);
void mmul(mat m1, mat m2, mat res);
void mvmul(mat m, vec v, vec res);

void mid(mat m);
void mtrans(vec v, mat m);
void mrotx(float r, mat m);
void mroty(float r, mat m);
void mrotz(float r, mat m);
void mscl(vec v, mat m);

// Number of matrices to allocate when we run out of space
#define MATSTACK_CHUNK 5
typedef struct {
	int nmats;
	mat *mats;
	int nmats_alloced;
} matstack;

void msprint(matstack *ms);
void mspush(matstack *ms, mat m);
void mspop(matstack *ms, mat res);
void msclear(matstack *ms);
void mscalc(matstack *ms, mat m);

#endif
