#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>
#include <stdlib.h>

#include "matrix.h"
#include "shapes.h"
#include "draw.h"

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

	shape *s = malloc(sizeof(shape));

	lua_pushstring(L, "npts");
	lua_gettable(L, -2);
	s->npts = lua_tonumber(L, -1);
	printf("npts: %i\n", s->npts);
	lua_pop(L, 1);
	s->pts = malloc(s->npts * sizeof(vec));
	
	lua_pushstring(L, "pts");
	lua_gettable(L, -2);
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_shape, didn't find a table for pts\n");
	for(int i = 0; i < s->npts; i++) {
		lua_pushnumber(L, i+1);
		lua_gettable(L, -2);
		lua_get_vec(L, s->pts[i]);
		vprint(s->pts[i]);
		printf("-----\n");
		lua_pop(L, 1);
	}
	lua_pop(L,1); // Pop the pts table

	lua_pushstring(L, "nlines");
	lua_gettable(L, -2);
	s->nlines = lua_tonumber(L, -1);
	printf("nlines: %i\n", s->nlines);
	lua_pop(L, 1);
	s->lines = malloc(s->nlines * sizeof(int)*2);
	
	lua_pushstring(L, "lines");
	lua_gettable(L, -2);
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_shape, didn't find a table for lines\n");
	for(int i = 0; i < s->nlines*2; i++) {
		lua_pushnumber(L, i+1);
		lua_gettable(L, -2);
		s->lines[i] = lua_tonumber(L, -1) -1; //Lua indexes from 1
		lua_pop(L, 1);
	}
	lua_pop(L,1); // Pop the line table

	lua_pop(L,1); // Pop the shape table

	print_shape(s);
	printf("%p\n", (void *)s);
	lua_pushlightuserdata(L, (void*)s);
	dumpstack(L);

	return 1;
}

static int lua_print_shape(lua_State *L) {
	shape *s = (shape *)lua_touserdata(L,-1);
	print_shape(s);
	return 0;
}

static int lua_show_shape(lua_State *L) {
	shape *s = (shape *)lua_touserdata(L,-1);
	show_shape(s);
	return 0;
}

static int lua_make_id_mat(lua_State *L) {
	mat *m = malloc(sizeof(mat));
	mid(*m);

	lua_pushlightuserdata(L, (void*)m);
	return 1;
}

static int lua_make_trans_mat(lua_State *L) {
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_trans_matrix, didn't find a table\n");

	mat *m = malloc(sizeof(mat));
	vec v;
	lua_get_vec(L, v);
	mtrans(v, *m);

	lua_pushlightuserdata(L, (void*)m);
	return 1;
}

static int lua_make_scl_mat(lua_State *L) {
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_scl_matrix, didn't find a table\n");

	mat *m = malloc(sizeof(mat));
	vec v;
	lua_get_vec(L, v);
	mscl(v, *m);

	lua_pushlightuserdata(L, (void*)m);
	return 1;
}

static int lua_make_rotx_mat(lua_State *L) {
	if(!lua_isnumber(L,-1))
		luaL_error(L, "lua_make_rotx_matrix, didn't find a number\n");

	mat *m = malloc(sizeof(mat));
	mrotx(lua_tonumber(L, -1), *m);

	lua_pushlightuserdata(L, (void*)m);
	return 1;
}

static int lua_make_roty_mat(lua_State *L) {
	if(!lua_isnumber(L,-1))
		luaL_error(L, "lua_make_roty_matrix, didn't find a number\n");

	mat *m = malloc(sizeof(mat));
	mroty(lua_tonumber(L, -1), *m);

	lua_pushlightuserdata(L, (void*)m);
	return 1;
}

static int lua_make_rotz_mat(lua_State *L) {
	if(!lua_isnumber(L,-1))
		luaL_error(L, "lua_make_rotz_matrix, didn't find a number\n");

	mat *m = malloc(sizeof(mat));
	mrotz(lua_tonumber(L, -1), *m);

	lua_pushlightuserdata(L, (void*)m);
	return 1;
}

static int lua_mmul(lua_State *L) {
	mat *m1 = (mat *)lua_touserdata(L,-1);
	mat *m2 = (mat *)lua_touserdata(L,-2);
	mat *m3 = (mat *)lua_touserdata(L,-3);

	mmul(*m1,*m2,*m3);
	return 0;
	
}

static int lua_stransform(lua_State *L) {
	shape *s = (shape *)lua_touserdata(L,-1);
	mat *m = (mat *)lua_touserdata(L,-2);

	print_shape(s);
	stransform(s, *m);
	print_shape(s);
	return 0;
}


static const struct luaL_Reg mylib[] = {
	{"make_shape", lua_make_shape},
	{"print_shape", lua_print_shape},
	{"show_shape", lua_show_shape},
	{"make_mat_id", lua_make_id_mat},
	{"make_mat_rotx", lua_make_rotx_mat},
	{"make_mat_roty", lua_make_roty_mat},
	{"make_mat_rotz", lua_make_rotz_mat},
	{"make_mat_trans", lua_make_trans_mat},
	{"make_mat_scl", lua_make_scl_mat},
	{"make_mat_mul", lua_mmul},
	{"shape_transform", lua_stransform},

	{NULL, NULL}
	};

int luaopen_cubes(lua_State *L) {
	luaL_newlib(L, mylib);
	return 1;
}
