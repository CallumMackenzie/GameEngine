
v2 = Vector2:new(30, 212)

function printMethods (o)
	D2D.write("Printing table methods:\n")
	for key,value in pairs(o) do
		D2D.write("found member " .. key .. "\n");
	end
end

function init ()
	local v1 = Vector2:new(10, 2)
	local derived = Vector2:new(30, 1)

	local h2d = Hitbox2D:new(47, Vector2:new(50, 50))

	sprt = Sprite:new("SPRITE_1", "./Planet1.png")
	sprt:setHitbox2D(h2d)
	sprt:setXY(500, 100)

	function derived:someMethod (slf)
		return "wow it works\n"
	end

	v1:setX(100)
	v1:setY(5)
	v1:getX()
	v1:getY()
	v1:normalize()

	D2D.write(
	"Info from methods: " .. 
	derived:someMethod() .. 
	"v1: " .. tostring(v1) .. 
	"\nv2: " .. tostring(v2) .. 
	"\nadded: " .. tostring(v1 + v2) .. 
	"\nv1: " .. tostring(v1) .. 
	"\nv2: " .. tostring(v2) .. 
	"\nv1 magnitude: " .. v1:magnitude() ..
	"\nh2d: " .. tostring(h2d) .. 
	"\nh2d type: " .. h2d:type() .. 
	"\nh2d size: (" .. h2d:width() .. ", " .. h2d:height() .. ")" ..
	"\nsprt: " .. tostring(sprt) .. 
	"\nsprt pos(" .. sprt:getX() .. ", " .. sprt:getY() .. ")" ..
	"\n")
	D2D.setClearColour(0xffffff)
	-- D2D.setFullscreen()
end

addValueY = 2
addValueX = 1

function onUpdate () 
	sprt:setXY(sprt:getX() + addValueX, sprt:getY() + addValueY)

	if sprt:getX() + 47 * 2 >= 1600 then
		addValueX = -1
	end
	if sprt:getX() <= 0 then
		addValueX = 1
	end

	if sprt:getY() + 47 * 2 >= 800 then
		addValueY = -2
	end
	if sprt:getY() <= 0 then
		addValueY = 2
	end

	sprt:addToRender()
	D2D.render(false)
end

function onClose()

end