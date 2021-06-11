#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "matrix.h"
#include "shapes.h"


/************************************/
void test_mrotx() {
	printf("%s\n", __func__);
	mat m1;
	mrotx(M_PI/4, m1);
	mprint(m1);
	printf("----------\n");

	vec v1 = {1,1,0,1};
	vec v2 = {0,};
	mvmul(m1, v1, v2);
	vprint(v2);
	printf("----------\n");
}

void test_mroty() {
	printf("%s\n", __func__);
	mat m1;
	mroty(M_PI/4, m1);
	mprint(m1);
	printf("----------\n");

	vec v1 = {1,1,0,1};
	vec v2 = {0,};
	mvmul(m1, v1, v2);
	vprint(v2);
	printf("----------\n");
}

void test_mrotz() {
	printf("%s\n", __func__);
	mat m1;
	mrotz(M_PI/4, m1);
	mprint(m1);
	printf("----------\n");

	vec v1 = {1,1,0,1};
	vec v2 = {0,};
	mvmul(m1, v1, v2);
	vprint(v2);
	printf("----------\n");
}

void test() {
	mat m1, m2, m3;
	mid(m1);
	mid(m2);
	mmul(m1, m2, m3);
	mprint(m3);
	printf("----------\n");

	printf("trans\n");
	vec v1 = {2,3,4,1};
	mtrans(v1, m1);
	mprint(m1);
	printf("----------\n");
	
	printf("scl\n");
	vec v2 = {1,10,100,1};
	mscl(v2, m2);
	mprint(m2);
	printf("----------\n");

	mmul(m2, m1, m3);
	mprint(m3);
	printf("----------\n");

	test_mrotx();
	test_mroty();
	test_mrotz();

}

int main (void) {
	test();
	return 0;
}
