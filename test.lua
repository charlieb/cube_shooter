cubes = require 'cubes'

local shape = {npts=4; pts={{ 0,  0, 0,1},
							{ 0, 20, 0, 1},
							{20, 20, 0, 1},
							{20,  0, 0, 1}
							};
			   nlines=4; lines={1,2, 
								2,3,
								  3,4,
							  4,1}};

local cube = {npts=8; pts={
	{-0.5,-0.5,-0.5,1},
	{-0.5, 0.5,-0.5,1},
	{ 0.5, 0.5,-0.5,1},
	{ 0.5,-0.5,-0.5,1},
	{-0.5,-0.5, 0.5,1},
	{-0.5, 0.5, 0.5,1},
	{ 0.5, 0.5, 0.5,1},
	{ 0.5,-0.5, 0.5,1}};
	nlines=12;
	lines = {
		1,2,
		2,3,
		3,4,
		4,1,

		5,6,
		6,7,
		7,8,
		8,5,

		1,5,
		2,6,
		3,7,
		4,8}};

local cshape = cubes.make_shape(cube);
--cubes.print_shape(cshape);
local m = cubes.make_mat_id();
cubes.mat_scl(m, {100, 100, 100, 1});
cubes.mat_print(m);
print("-------------\n");
--cubes.mat_rotx(m, 0.5);
cubes.mat_rotz(m, 0.7);
cubes.mat_roty(m, 0.7);
--cubes.mat_print(m);
cubes.shape_transform(m, cshape);
print("-------------\n");
cubes.print_shape(cshape);

cubes.show_shape(cshape);

