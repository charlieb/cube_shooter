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

local m = cubes.make_mat_id();

function mat_test() 
	cubes.mat_scl(m, {100, 100, 100, 1});
	--cubes.mat_print(m);
	--print("-------------\n");
	--cubes.mat_rotx(m, 0.5);
	cubes.mat_rotz(m, 0.7);
	cubes.mat_roty(m, 0.7);
	--cubes.mat_print(m);

	cubes.mat_set_id(m);
	cubes.mat_print(m);
	cubes.mat_trans(m, {2,3,4,1});
	local cms = cubes.make_matstack();
	cubes.matstack_push(cms, m);
	cubes.matstack_print(cms)

	cubes.mat_set_id(m);
	cubes.mat_scl(m, {10, 100, 1000, 1});
	cubes.matstack_push(cms, m);
	cubes.matstack_print(cms)
	cubes.matstack_calc(cms, m);
	print("Calc -------------");
	cubes.mat_print(m);

	cubes.mat_set_id(m);
	print("ID -------------");
	cubes.mat_print(m);

	cubes.matstack_pop(cms, m);
	print("Pop -------------\n");
	cubes.mat_print(m);

	cubes.matstack_clear(cms);
	cubes.matstack_pop(cms, m);
	print("Clear -------------\n");
	cubes.mat_print(m);
	cubes.matstack_print(cms)
end 


function shape_test()
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
	local csms = cubes.shape_matstack(cshape);
	local s = 100;
	cubes.mat_set_id(m);
	cubes.mat_scl(m, {s, s, s, 1});
	cubes.matstack_push(csms, m);

	cubes.mat_set_id(m);
	cubes.mat_rotz(m, 0.7);
	cubes.matstack_push(csms, m);

	local ry = 0
	cubes.mat_set_id(m);
	cubes.mat_roty(m, ry);
	cubes.matstack_push(csms, m);
	--cubes.mat_print(m);
end
function test_show_shape()
	local si = -1;
	local quit = false
	while not quit do
		ry = ry + 0.01;
		cubes.mat_set_id(m);
		cubes.mat_roty(m, ry);
		cubes.matstack_poke(csms, 2, m);

		if s > 100 or s < 20 then
			si = -si;
		end 
		s = s + si;
		cubes.mat_set_id(m);
		cubes.mat_scl(m, {s,s,s,1});
		cubes.matstack_poke(csms, 0, m);

		quit = cubes.show_shape(cshape)
	end 
end
--test_show_shape()

fs = {}
for i=1,10 do
	fs[i] = cubes.make_fixture()
end

print "Fixtures Made"

cubes.fixture_add_child(fs[1], fs[2])
cubes.fixture_add_child(fs[1], fs[3])
cubes.fixture_add_child(fs[1], fs[4])

cubes.fixture_add_child(fs[3], fs[5])
cubes.fixture_add_child(fs[3], fs[6])
cubes.fixture_add_child(fs[3], fs[7])

cubes.fixture_add_child(fs[6], fs[7])
cubes.fixture_add_child(fs[6], fs[8])
cubes.fixture_add_child(fs[6], fs[9])
cubes.fixture_add_child(fs[8], fs[10])

cubes.fixture_show(fs[1])
