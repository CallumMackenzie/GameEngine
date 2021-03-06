
function init ()
	local v1 = Vector2.new(10, 2)
	local v2 = Vector2.new(30, 212)
	local h2d = Hitbox2D.new(v1, v2)
	D2D.write(tostring(h2d), "\n")
	D2D.setSize(1600, 900)
end

function onUpdate () 
	D2D.render(true)
end

function onClose()

end