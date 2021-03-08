
v2 = Vector2:new(30, 212)

function printMethods (o)
	D2D.write("Searching table\n");
	for key,value in pairs(o) do
		D2D.write("	found member " .. key .. "\n");
	end
end

function init ()
	local v1 = Vector2:new(10, 2)
	local derived = Vector2:new(30, 1)

	function derived:someMethod (slf)
		return "wow it works\n"
	end

	local hb2d = Hitbox2D:new(10, v2)

	-- local h2d = Hitbox2D.new(v1, v2)
	printMethods(v1)
	printMethods(getmetatable(v1))

	v1:setX(100)
	v1:setY(5)
	v1:getX()
	v1:getY()

	D2D.write(
	"Info from methods: " .. 
	derived:someMethod() .. 
	"v1: " .. tostring(v1) .. 
	"\nv2: " .. tostring(v2) .. 
	"\nadded: " .. tostring(v1 + v2) .. 
	"\nv1: " .. tostring(v1) .. 
	"\nv2: " .. tostring(v2) .. 
	"\nv1 magnitude: " .. v1:magnitude() .. "\n")
	v1:normalize()
	D2D.write(tostring(v1) .. "\n")
	D2D.setSize(1600, 900)
end

function onUpdate () 
	local tempVector = Vector2:new(1, 2)
	D2D.render(true)
end

function onClose()

end