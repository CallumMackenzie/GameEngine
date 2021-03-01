#include "Memory.h"

#include "Hitbox2D.h"

Hitbox2D Hitbox2D::createUndefinedHitbox()
{
	return Hitbox2D(TYPE_UNDEFINED);
}

Hitbox2D Hitbox2D::createCircleHitbox(float diameter, Vector2 pos)
{
	Hitbox2D h2d = Hitbox2D(TYPE_CIRCLE);
	h2d.points[0] = Vector2(diameter, 0);
	h2d.points[1] = pos;

	return h2d;
}

Hitbox2D Hitbox2D::createRectHitbox(Vector2 pos, Vector2 size_)
{
	Hitbox2D h2d = Hitbox2D (TYPE_RECTANGLE);

	h2d.points[0] = pos; // Top left
	h2d.points[1] = Vector2(pos.x() + size_.x(), pos.y()); // Top right
	h2d.points[2] = Vector2(pos.x(), pos.y() + size_.y()); // Bottom left
	h2d.points[3] = Vector2(pos.x() + size_.x(), pos.y() + size_.y()); // Bottom right

	return h2d;
}

Hitbox2D::Hitbox2D(int type_)
{
	type = type_;
}

Vector2 Hitbox2D::rectSize()
{
	return Vector2::subtract(points[3], points[0]);
}

Vector2 Hitbox2D::rectPos()
{
	return Vector2::add(points[0], position);
}

Vector2 Hitbox2D::rectUL()
{
	return rectPos();
}

Vector2 Hitbox2D::rectUR()
{
	return Vector2::add(rectPos(), Vector2(rectSize().x(), 0));
}

Vector2 Hitbox2D::rectBR()
{
	return Vector2::add(rectPos(), rectSize());
}

Vector2 Hitbox2D::rectBL()
{
	return Vector2::add(rectPos(), Vector2(0, rectSize().y()));
}

float Hitbox2D::circleRadius()
{
	return points[0].x();
}

Vector2 Hitbox2D::circleCentre()
{
	return Vector2::add(points[1], position);
}