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
void mrotz(float r, mat m);




#endif
