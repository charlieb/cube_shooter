#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>

#include "matrix.h"

static void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

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

void lua_check_return(lua_State *L, int error) {
	if (error) {
		fprintf(stderr, "C: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);  /* pop error message from the stack */
	}
}

int lua_get_vec(lua_State *L, vec v) {
	if(!lua_istable(L,-1)) 
		luaL_error(L, "lua_get_vec, didn't find a table\n");
	for(int i = 0; i < 4; i++) {
		lua_pushnumber(L, i+1);
		lua_gettable(L, -2);
		if(!lua_isnumber(L, -1))
			luaL_error(L, "lua_get_vec, didn't find a number in the vec table\n");
		v[i] = lua_tonumber(L, -1);
		lua_pop(L,1);
	}
	return 0;
}

int lua_make_shape(lua_State *L) {
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_shape, didn't find a table\n");
	vec v;
	lua_get_vec(L, v);
	
	vprint(v);
	/*
	lua_pushstring(L, "pts");
	lua_gettable(L, -2);
	if(!lua_istable(L,-1)) {
	}
	for
	*/
	return 0;
}

void test_lua() {
	char test[] = "make_shape({1.5,2.5,3.5,1});";

	lua_State *L = luaL_newstate();   /* opens Lua */
	luaL_openlibs(L);

	lua_pushcfunction(L, lua_make_shape);
	lua_setglobal(L, "make_shape");

	int error = luaL_loadbuffer(L, test, strlen(test), "line") ||
		lua_pcall(L, 0, 0, 0);
	if (error) {
		fprintf(stderr, "C: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);  /* pop error message from the stack */
	}

	lua_close(L);
}

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

	vec v1 = {2,3,4,1};
	mtrans(v1, m1);
	mprint(m1);
	printf("----------\n");

	mmul(m1, m2, m3);
	mprint(m3);
	printf("----------\n");

	test_mrotx();
	test_mroty();
	test_mrotz();

}

int main (void) {
	test();
	test_lua();
	return 0;
}
