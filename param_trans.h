#ifndef _PARAM_TRANS_
#define _PARAM_TRANS_

#include "matrix.h"
typedef struct {
	int loc[2];
	int nvalues;
	float *values;
} param;

typedef struct {
	mat base;
	int nparams;
	param *params;
} params;

void apply_params(params *p, mat res, int t);


#endif
