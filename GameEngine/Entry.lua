
v2 = Vector2:new(30, 212)

function init ()
	local v1 = Vector2:new(10, 2)
	local derived = Vector2:new(30, 1)

	function derived:someMethod (slf)
		return "wow it works\n"
	end

	-- local h2d = Hitbox2D.new(v1, v2)

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
	D2D.setClearColour(0x000000)
	D2D.setFullscreen()
end

function onUpdate () 
	D2D.render(false)
end

function onClose()

end