#include "param_trans.h"

#include <string.h>

void apply_params(params *p, mat res, int t) {
	memcpy(res, p->base, sizeof(mat));
	for(int i = 0; i < p->nparams; i++)
		res[p->params[i].loc[0]][p->params[i].loc[1]] =
			p->params[i].values[t%p->params[i].nvalues];
}
