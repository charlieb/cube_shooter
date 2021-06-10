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
	{0,0,0,1},
	{0,1,0,1},
	{1,1,0,1},
	{1,0,0,1},
	{0,0,1,1},
	{0,1,1,1},
	{1,1,1,1},
	{1,0,1,1}};
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
local cmrz = cubes.make_mat_rotz(0.7);
cubes.shape_transform(cmrz, cshape);
local cmrx = cubes.make_mat_rotx(1.7);
cubes.shape_transform(cmrx, cshape);
local cmscl = cubes.make_mat_scl({100,100,100,1});
cubes.shape_transform(cmscl, cshape);
cubes.show_shape(cshape);
