cubes = require 'cubes'

local shape = {npts=2; pts={{1.1,2.2,3.3,1}, {4.4, 5.5, 6.6, 1}};
						   nlines=1; lines={1,2}};
local cshape = cubes.make_shape(shape);
print(cshape);
cubes.print_shape(cshape);
