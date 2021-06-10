#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>
#include <stdlib.h>

#include "matrix.h"
#include "shapes.h"

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


void lua_check_return(lua_State *L, int error) {
	if (error) {
		fprintf(stderr, "C: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);  /* pop error message from the stack */
	}
}

static void lua_get_vec(lua_State *L, vec v) {
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
}

static int lua_make_shape(lua_State *L) {
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_shape, didn't find a table\n");

	shape s;

	dumpstack(L);
	lua_pushstring(L, "npts");
	lua_gettable(L, -2);
	int npts = lua_tonumber(L, -1);
	printf("npts: %i\n", npts);
	lua_pop(L, 1);
	s.pts = malloc(npts * sizeof(vec));
	
	dumpstack(L);
	lua_pushstring(L, "pts");
	lua_gettable(L, -2);
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_shape, didn't find a table for pts\n");
	for(int i = 0; i < npts; i++) {
		lua_pushnumber(L, i+1);
		lua_gettable(L, -2);
		lua_get_vec(L, s.pts[i]);
		vprint(s.pts[i]);
		lua_pop(L, 1);
	}
	return 0;
}

static const struct luaL_Reg mylib[] = {
	{"make_shape", lua_make_shape},
	{NULL, NULL}
	};

int luaopen_cubes(lua_State *L) {
	luaL_newlib(L, mylib);
	return 1;
}
