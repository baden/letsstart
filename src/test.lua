-----
require"obj"

--dofile("obj.lua");



print(string.format("%30.25f", math.pi));
print(string.byte("a"));

function printf(...)
 io.write(string.format(...))
end




player = obj {
	name="It's me",
	x=0, y=0,
	hit = function(self)
		print("My Å´† own hit procedure");
		return 1;
--		call(self, "hit");
	end
}


trademan = obj {
 name="ShopMan",
 x=0, y=0
}

player.x = player.x + 1

print("Playes position: X:", player.x, " Y:", player.y);

print("HITON:", player.hit(trademan));
print("HITON:", trademan.hit(player));


-- io.write("Hello world, from ", jit.version, "!\n");
-- print("Hello world, from ", jit.version, "!\n");
print("Hello world, from lua!\n");

--a1 = 123;
--a2 = "123";
--a3 = {123};
--a4 = print;
--a5 = player;

--print("type of a1=" .. type(a1) .. "  a1=" .. a1);
--print("type of a2=" .. type(a2) .. "  a2=" .. a2);
--print("type of a3=" .. type(a3) .. "  a3=" );
--print("type of a4=" .. type(a4) .. "  a4=" );
--print("type of a5=" .. type(a5) .. "  a5=" .. a5.z);


--local i;
--local j;
--local k=0;

--for i=0,10000 do
-- for j=0,10000 do
--  k=k+j*j/10000+i*i/10000
-- end
--end
--print("k=", k, "\n");

local function add(a,b) return a + b end
local x = add(1,add(2,3))
print("x=", x)


local lib = {}
function lib:Install ()
-- local old = getglobal(self.name)
-- print("Self name: ", old)
 print("Lib installed\n")
end

lib:Install()

printf("Hello %s from %s on %s\n",os.getenv"USER" or "there",_VERSION,os.date())
