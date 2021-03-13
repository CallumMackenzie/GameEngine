function printMethods (o)
	D2D.write("Printing table methods:\n")
	for key,value in pairs(o) do
		D2D.write("found member " .. key .. "\n");
	end
end

function init ()
	D2D.show()

	local h2d = Hitbox2D:new(47, Vector2:new(50, 50))

	sprt = Sprite:new("SPRITE_1", "./Planet1.png")
	sprt:setSize(100, 100)
	sprt:setHitbox2D(h2d)
	sprt:setXY(500, 100)
	sprt:setRotationCenter(50, 50)
	sprt:renderHitbox(false)

	local sfd = SpriteFrameData:new(12, false, 40, 64, 0.1)
	runner = Sprite:new("Runner", "./RunSpriteSheet.png", sfd)
	runner:setHitbox2D(h2d)
	runner:setSize(80, 128)
	runner:setXY(200, 200)
	runner:setRotationCenter(40, 64)

	addValue = Vector2:new()
	deceleration = Vector2:new(0.9, 0.9)
	accel = 0.5

	function sprt:manageMovement (slf)
		if D2D.keyPressed(87) then
			addValue:setY(-accel)
		end
		if D2D.keyPressed(83) then
			addValue:setY(accel)
		end
		if D2D.keyPressed(68) then
			addValue:setX(accel)
		end
		if D2D.keyPressed(65) then
			addValue:setX(-accel)
		end
		return
	end

	D2D.setClearColour(0xffffff)
	Time.setFPS(200)
	D2D.setFullscreen()
end

function onFixedUpdate() 
	addValue = addValue * deceleration
end

function onUpdate () 
	sprt:manageMovement()

	sprt:addXY(addValue:getX() * Time.deltaTime(), addValue:getY() * Time.deltaTime())
	sprt:addRotation(0, 0, 0.1 * Time.deltaTime())

	local coll = sprt:getCollision(runner)
	if coll:getDirection() > 0 then
		sprt:addXY(coll:getHitVectorX() / 2, coll:getHitVectorY() / 2)
		runner:addXY(-coll:getHitVectorX() / 2, -coll:getHitVectorY() / 2)
	end

	runner:calculateFrame()

	sprt:render()
	runner:render()
	D2D.render(false)
end

function onClose()

end