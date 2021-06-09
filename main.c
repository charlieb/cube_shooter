#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>

#include "matrix.h"

void lua() {
	char buff[256];
	int error;
	lua_State *L = luaL_newstate();   /* opens Lua */
	luaL_openlibs(L);

	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
			lua_pcall(L, 0, 0, 0);
		if (error) {
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);  /* pop error message from the stack */
		}
	}

	lua_close(L);
}

void test() {
	mat m1, m2, m3;
	mid(m1);
	mid(m2);
	mmul(m1, m2, m3);
	mprint(m3);
	printf("----------\n");

	vec v1 = {2,3,4,1};
	mtrans(v1, m1);
	mprint(m1);
	printf("----------\n");

	mmul(m1, m2, m3);
	mprint(m3);
	printf("----------\n");

	mid(m1);
	mrotz(M_PI/4, m1);
	mprint(m1);
	printf("----------\n");

	mmul(m1, m2, m3);
	mprint(m3);
	printf("----------\n");

	vec v2 = {1,1,0,1};
	vec v3 = {0,};
	mvmul(m3,v2, v3);
	vprint(v3);

}

int main (void) {
	test();
	return 0;
}
