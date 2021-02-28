#include "Memory.h"

#include "Sprite.h"

Sprite::Sprite(const char* name_, Vector2 pos, Rotation rot, ID2D1Bitmap* bitmap_, Hitbox2D hb2d) : Renderable<ID2D1Bitmap>(name_, pos, rot, bitmap_)
{
	hitbox2D = hb2d;
	bitmap = bitmap_;
	setXY(pos);
}

void Sprite::setXY(float x, float y)
{
	position.x(x);
	position.y(y);
	hitbox2D.position.x(x);
	hitbox2D.position.y(y);
}

void Sprite::setXY(Vector2 pos)
{
	position.reassign(pos);
	hitbox2D.position.reassign(pos);
}

void Sprite::subtractXY(Vector2 pos)
{
	position.subtract(pos);
	hitbox2D.position.subtract(pos);
}

void Sprite::subtractXY(float x, float y)
{
	subtractXY(Vector2(x, y));
}

void Sprite::addXY(float x, float y)
{
	addXY(Vector2(x, y));
}

void Sprite::addXY(Vector2 pos)
{
	position.add(pos);
	hitbox2D.position.add(pos);
}

void Sprite::setY(float y) {
	setXY(position.x(), y);
}
void Sprite::setX(float x) {
	setXY(x, position.y());
}