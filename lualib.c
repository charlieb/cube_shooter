#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

/*TODO consider why use a mat* instead of just a mat which is a pointer itself*/
static int lua_make_id_mat(lua_State *L) {
	mat *m = malloc(sizeof(mat));
	mid(*m);

	lua_pushlightuserdata(L, (void*)m);
	return 1;
}

static int lua_set_id_mat(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	mid(*m);
	return 0;
}

static int lua_trans_mat(lua_State *L) {
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_trans_matrix, didn't find a table\n");

	mat *m = (mat *)lua_touserdata(L,-2);
	mat tr,res;
	vec v;
	lua_get_vec(L, v);

	mtrans(v, tr);
	//mmul(*m, tr, res);
	mmul(tr, *m, res);
	memcpy(*m, res, sizeof(mat));

	return 0;
}

static int lua_scl_mat(lua_State *L) {
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_scl_matrix, didn't find a table\n");

	mat *m = (mat *)lua_touserdata(L,-2);
	mat tr,res;
	vec v;
	lua_get_vec(L, v);

	mscl(v, tr);
	mmul(tr, *m, res);
	memcpy(*m, res, sizeof(mat));
	return 0;
}

static int lua_rot_mat(lua_State *L, void rotfn(float, mat)) {
	if(!lua_isnumber(L,-1))
		luaL_error(L, "lua_make_rot_matrix, didn't find a number\n");

	mat *m = (mat *)lua_touserdata(L,-2);
	mat tr,res;
	rotfn(lua_tonumber(L, -1), tr);
	mmul(tr, *m, res);
	memcpy(*m, res, sizeof(mat));
	return 0;
}
static int lua_rotx_mat(lua_State *L) {return lua_rot_mat(L, mrotx); }
static int lua_roty_mat(lua_State *L) {return lua_rot_mat(L, mroty); }
static int lua_rotz_mat(lua_State *L) {return lua_rot_mat(L, mrotz); }

static int lua_mmul(lua_State *L) {
	mat *m1 = (mat *)lua_touserdata(L,-1);
	mat *m2 = (mat *)lua_touserdata(L,-2);
	mat *m3 = (mat *)lua_touserdata(L,-3);

	mmul(*m1,*m2,*m3);
	return 0;
}

static int lua_mat_print(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);

	mprint(*m);
	return 0;
}

static int lua_make_matstack(lua_State *L) {
	matstack *ms = malloc(sizeof(matstack));
	memset(ms, 0, sizeof(matstack));

	lua_pushlightuserdata(L, (void*)ms);
	return 1;
}

static int lua_matstack_push(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	matstack *ms = (matstack *)lua_touserdata(L,-2);
	mspush(ms, *m);
	return 0;
}

static int lua_matstack_pop(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	matstack *ms = (matstack *)lua_touserdata(L,-2);
	mspop(ms, *m);
	return 0;
}

static int lua_matstack_calc(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	matstack *ms = (matstack *)lua_touserdata(L,-2);
	mscalc(ms, *m);
	return 0;
}

static int lua_matstack_clear(lua_State *L) {
	matstack *ms = (matstack *)lua_touserdata(L,-1);
	msclear(ms);
	return 0;
}

static int lua_matstack_poke(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	int n = lua_tonumber(L,-2);
	matstack *ms = (matstack *)lua_touserdata(L,-3);
	mspoke(ms, n, *m);
	return 0;
}

static int lua_matstack_peek(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	int n = lua_tonumber(L,-2);
	matstack *ms = (matstack *)lua_touserdata(L,-3);
	mspeek(ms, n, *m);
	return 0;
}

static int lua_matstack_print(lua_State *L) {
	matstack *ms = (matstack *)lua_touserdata(L,-1);

	msprint(ms);
	return 0;
}

/************ SHAPES *****************/

static int lua_make_proto_shape(lua_State *L) {
	if(!lua_istable(L,-1))
		luaL_error(L, "lua_make_shape, didn't find a table\n");

	proto_shape *s = malloc(sizeof(shape));
	memset(s, 0, sizeof(shape));

	lua_pushstring(L, "npts");
	lua_gettable(L, -2);
	s->npts = lua_tonumber(L, -1);
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
		lua_pop(L, 1);
	}
	lua_pop(L,1); // Pop the pts table

	lua_pushstring(L, "nlines");
	lua_gettable(L, -2);
	s->nlines = lua_tonumber(L, -1);
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

	lua_pushlightuserdata(L, (void*)s);

	return 1;
}

static int lua_make_shape_from(lua_State *L) {
	proto_shape *ps = (proto_shape *)lua_touserdata(L,-1);
	shape *s = malloc(sizeof(shape));
	s->proto = ps;
	s->npts = ps->npts;
	s->pts = malloc(s->npts * sizeof(vec));
	memcpy(s->pts, ps->pts, s->npts * sizeof(vec));
	lua_pushlightuserdata(L, (void *)s);
	return 0;
}

static int lua_print_shape(lua_State *L) {
	shape *s = (shape *)lua_touserdata(L,-1);
	print_shape(s);
	return 0;
}

static int lua_stransform(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	shape *s = (shape *)lua_touserdata(L,-2);

	stransform(s, *m);
	return 0;
}

/************ FIXTURE **************/

static int lua_fixture_matstack(lua_State *L) {
	fixture *f = (fixture *)lua_touserdata(L,-1);
	lua_pushlightuserdata(L, &f->ms);
	return 1;
}

static int lua_fixture_matstack_push(lua_State *L) {
	mat *m = (mat *)lua_touserdata(L,-1);
	fixture *f = (fixture *)lua_touserdata(L,-2);
	fmspush(f, *m);
	return 0;
}

static int lua_make_fixture(lua_State *L) {
	fixture *f = malloc(sizeof(fixture));
	memset(f, 0, sizeof(fixture));
	lua_pushlightuserdata(L, (void *)f);
	return 1;
}

static int lua_fixture_add_child(lua_State *L) {
	fixture *f = (fixture *)lua_touserdata(L,-2);
	fixture *child = (fixture *)lua_touserdata(L,-1);
	fpush(f, child);
	return 0;
}

static int lua_fixture_set_shape(lua_State *L) {
	shape *s = (shape *)lua_touserdata(L,-1);
	fixture *f = (fixture *)lua_touserdata(L,-2);
	f->s = s;
	s->fixture = f;
	return 0;
}

static int lua_fixture_show(lua_State *L) {
	fixture *f = (fixture *)lua_touserdata(L,-1);
	shape **shapes = NULL;
	int nshapes = 0;
	frender(f, &nshapes, &shapes);
	show_shapes(nshapes, shapes);
	return 0;
}

static const struct luaL_Reg mylib[] = {
	{"make_mat_id", lua_make_id_mat},
	{"mat_set_id", lua_set_id_mat},
	{"mat_rotx", lua_rotx_mat},
	{"mat_roty", lua_roty_mat},
	{"mat_rotz", lua_rotz_mat},
	{"mat_trans", lua_trans_mat},
	{"mat_scl", lua_scl_mat},
	{"mat_mul", lua_mmul},
	{"mat_print", lua_mat_print},

	{"make_matstack", lua_make_matstack},
	{"matstack_push", lua_matstack_push},
	{"matstack_pop", lua_matstack_pop },
	{"matstack_clear", lua_matstack_clear},
	{"matstack_poke", lua_matstack_poke},
	{"matstack_peek", lua_matstack_peek},
	{"matstack_calc", lua_matstack_calc},
	{"matstack_print", lua_matstack_print},

	{"make_shape", lua_make_proto_shape},
	{"make_shape_from", lua_make_shape_from},
	{"print_shape", lua_print_shape},
	{"shape_transform", lua_stransform},

	{"make_fixture", lua_make_fixture},
	{"fixture_add_child", lua_fixture_add_child},
	{"fixture_matstack", lua_fixture_matstack},
	{"fixture_matstack_push", lua_fixture_matstack_push},
	{"fixture_set_shape", lua_fixture_set_shape},
	{"fixture_show", lua_fixture_show},

	{NULL, NULL}
	};

// Used when the .so library is loaded from lua
int luaopen_cubes(lua_State *L) {
	luaL_newlib(L, mylib);
	return 1;
}

