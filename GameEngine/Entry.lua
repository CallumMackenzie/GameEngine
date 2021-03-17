-- blocks = {}

-- for i = 1, 10 do
--	blocks[i] = Sprite:new("BLOCK", "D:/Images/Wood.png")
--	blocks[i]:setSize(32.5, 77)
--	blocks[i]:setHitbox2D(Hitbox2D:new(Vector2:new(), Vector2:new(32.5, 77)))
--	blocks[i]:setXY(750, i * 77)
--	blocks[i]:renderHitbox(false)
-- end

function onCreate ()
	-- D2D.show()
	D2D.setCameraPos(0, 0)
	D2D.setCameraZoom(1, 1)

	sprt = Sprite:new("SPRITE_1", "./Planet1.png")
	sprt:setSize(100, 100)
	sprt:setHitbox2D(Hitbox2D:new(46, Vector2:new(50, 50)))
	sprt:setXY(0, 0)
	sprt:setRotationCenter(50, 50)

	runner = Sprite:new("Runner", "./RunSpriteSheet.png", SpriteFrameData:new(12, false, 40, 64, 0.1))
	runner:setHitbox2D(Hitbox2D:new(47, Vector2:new(50, 50)))
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
	-- D2D.setFullscreen()
end

function onFixedUpdate() 
	-- local vec = Vector2:new(5 / Time.fixedDeltaTime(),  5 / Time.fixedDeltaTime())
	addValue = addValue * deceleration
end

function onUpdate () 
	-- D2D.addCameraPos(0.1 * Time.deltaTime(), 0.1 * Time.deltaTime())

	sprt:manageMovement()
	sprt:addXY(addValue:getX() * Time.deltaTime(), addValue:getY() * Time.deltaTime())
	sprt:addRotation(0, 0, 0.1 * Time.deltaTime())
	local coll = sprt:getCollision(runner)
	if coll:getDirection() > 0 then
		-- sprt:addXY(coll:getHitVectorX() / 2, coll:getHitVectorY() / 2)
		-- runner:addXY(-coll:getHitVectorX() / 2, -coll:getHitVectorY() / 2)
		runner:addXY(-coll:getHitVectorX(), -coll:getHitVectorY())
	end

	if runner:getX() > 1610 then
		runner:setX(-50)
	end
	-- for i = 1, 10 do
		-- local coll = blocks[i]:getCollision(sprt)
		-- if coll:getDirection() > 0 then
			-- sprt:addXY(-coll:getHitVectorX(), -coll:getHitVectorY())
		-- end
		-- blocks[i]:render()
	-- end
	runner:calculateFrame()
	Line:new(Vector2:new(0, 0), Vector2:new(sprt:getX() + 50, sprt:getY() + 50), 10, 0xbd8af1, 0.5):render()
	sprt:render()
	runner:render()
	D2D.render(false)
end

function onClose()
	
end