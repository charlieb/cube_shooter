#include "matrix.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void mprint(mat m) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) 
			printf("%4f ", m[j][i]);
		printf("\n");
	}
}

void vprint(vec v) {
	for(int i = 0; i < 4; i++)
		printf("%4f\n", v[i]);
}

void mmul(mat m1, mat m2, mat res) {
	res[0][0] =
		m1[0][0] * m2[0][0] +
		m1[1][0] * m2[0][1] +
		m1[2][0] * m2[0][2] +
		m1[3][0] * m2[0][3];
                       
	res[0][1] =
		m1[0][1] * m2[0][0] +
		m1[1][1] * m2[0][1] +
		m1[2][1] * m2[0][2] +
		m1[3][1] * m2[0][3];

	res[0][2] =
		m1[0][2] * m2[0][0] +
		m1[1][2] * m2[0][1] +
		m1[2][2] * m2[0][2] +
		m1[3][2] * m2[0][3];
                      
	res[0][3] =
		m1[0][3] * m2[0][0] +
		m1[1][3] * m2[0][1] +
		m1[2][3] * m2[0][2] +
		m1[3][3] * m2[0][3];

	/************************/
	res[1][0] =
		m1[0][0] * m2[1][0] +
		m1[1][0] * m2[1][1] +
		m1[2][0] * m2[1][2] +
		m1[3][0] * m2[1][3];

	res[1][1] =
		m1[0][1] * m2[1][0] +
		m1[1][1] * m2[1][1] +
		m1[2][1] * m2[1][2] +
		m1[3][1] * m2[1][3];

	res[1][2] =
		m1[0][2] * m2[1][0] +
		m1[1][2] * m2[1][1] +
		m1[2][2] * m2[1][2] +
		m1[3][2] * m2[1][3];
                      
	res[1][3] =
		m1[0][3] * m2[1][0] +
		m1[1][3] * m2[1][1] +
		m1[2][3] * m2[1][2] +
		m1[3][3] * m2[1][3];
	/************************/
	res[2][0] =
		m1[0][0] * m2[2][0] +
		m1[1][0] * m2[2][1] +
		m1[2][0] * m2[2][2] +
		m1[3][0] * m2[2][3];

	res[2][1] =
		m1[0][1] * m2[2][0] +
		m1[1][1] * m2[2][1] +
		m1[2][1] * m2[2][2] +
		m1[3][1] * m2[2][3];

	res[2][2] =
		m1[0][2] * m2[2][0] +
		m1[1][2] * m2[2][1] +
		m1[2][2] * m2[2][2] +
		m1[3][2] * m2[2][3];
                      
	res[2][3] =
		m1[0][3] * m2[2][0] +
		m1[1][3] * m2[2][1] +
		m1[2][3] * m2[2][2] +
		m1[3][3] * m2[2][3];

	/************************/
	res[3][0] =
		m1[0][0] * m2[3][0] +
		m1[1][0] * m2[3][1] +
		m1[2][0] * m2[3][2] +
		m1[3][0] * m2[3][3];

	res[3][1] =
		m1[0][1] * m2[3][0] +
		m1[1][1] * m2[3][1] +
		m1[2][1] * m2[3][2] +
		m1[3][1] * m2[3][3];

	res[3][2] =
		m1[0][2] * m2[3][0] +
		m1[1][2] * m2[3][1] +
		m1[2][2] * m2[3][2] +
		m1[3][2] * m2[3][3];
                      
	res[3][3] =
		m1[0][3] * m2[3][0] +
		m1[1][3] * m2[3][1] +
		m1[2][3] * m2[3][2] +
		m1[3][3] * m2[3][3];
}

void mvmul(mat m, vec v, vec res) {
	res[0] = 
		m[0][0] * v[0] + 
		m[1][0] * v[1] +
		m[2][0] * v[2] +
		m[3][0] * v[3];
                          
	res[1] = 
		m[0][1] * v[0] + 
		m[1][1] * v[1] +
		m[2][1] * v[2] +
		m[3][1] * v[3];
                          
	res[2] = 
		m[0][2] * v[0] + 
		m[1][2] * v[1] +
		m[2][2] * v[2] +
		m[3][2] * v[3];

	res[3] = 
		m[0][3] * v[0] + 
		m[1][3] * v[1] +
		m[2][3] * v[2] +
		m[3][3] * v[3];
}

void mid(mat m) {
    mat mid = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	memcpy(m, mid, sizeof(mat));
}

void mtrans(vec v, mat m) {
	mid(m);
	m[3][0] = v[0];
	m[3][1] = v[1];
	m[3][2] = v[2];
}

void mrotx(float r, mat m) {
	mid(m);
	m[1][1] = cos(r);
	m[1][2] = -sin(r);
	m[2][1] = sin(r);
	m[2][2] = cos(r);
}

void mroty(float r, mat m) {
	mid(m);
	m[0][0] = cos(r);
	m[0][2] = -sin(r);
	m[2][0] = sin(r);
	m[2][2] = cos(r);
}

void mrotz(float r, mat m) {
	mid(m);
	m[0][0] = cos(r);
	m[0][1] = -sin(r);
	m[1][0] = sin(r);
	m[1][1] = cos(r);
}

void mscl(vec v, mat m) {
	mid(m);
	m[0][0] = v[0];
	m[1][1] = v[1];
	m[2][2] = v[2];
}

void msprint(matstack *ms) {
	printf("nmats: %i, nmats_alloced: %i\n", 
			ms->nmats, ms->nmats_alloced);
	for(int i = 0; i < ms->nmats; i++) {
		mprint(ms->mats[i]);
		printf("-----------\n");
	}
}
void mspush(matstack *ms, mat m) {
	if(ms->nmats == ms->nmats_alloced) {
		ms->nmats_alloced += MATSTACK_CHUNK;
		ms->mats = realloc(ms->mats, ms->nmats_alloced * sizeof(mat));
	}
	memcpy(ms->mats[ms->nmats], m, sizeof(mat));
	ms->nmats++;
}
void mspop(matstack *ms, mat res) {
	memcpy(res, ms->mats[--ms->nmats], sizeof(mat));
}

void msclear(matstack *ms) {
	ms->nmats = 0;
}
void mscalc(matstack *ms, mat res) {
	mat tmp[2];
	mid(tmp[0]); mid(tmp[1]);
	for(int i = 0; i < ms->nmats; i++) {
		printf("=============\n");
		mprint(tmp[0]);
		printf("-------------\n");
		mprint(tmp[1]);
		printf("-------------\n");
		mmul(tmp[i%2], ms->mats[i], tmp[(i+1)%2]);
		mprint(tmp[0]);
		printf("-------------\n");
		mprint(tmp[1]);
		printf("=============\n");
	}
	memcpy(res, tmp[(ms->nmats)%2], sizeof(mat));
}

