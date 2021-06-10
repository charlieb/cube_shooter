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
local cshape = cubes.make_shape(shape);
local cmrz = cubes.make_mat_rotz(0.7);
cubes.shape_transform(cmrz, cshape);
cubes.show_shape(cshape);
