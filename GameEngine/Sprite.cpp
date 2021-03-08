#include "Memory.h"
#include "ModWin.h"
#include "Sprite.h"


Sprite::Sprite(const char* name_, LPCWSTR bitmapPath, FrameData fd, ID2D1RenderTarget* pRT, Hitbox2D hb2d = Hitbox2D::createUndefinedHitbox(), 
	Vector2 pos = Vector2(), Rotation rot = Rotation()) : Sprite(name_, bitmap, hb2d, pos, rot)
{
	loadFileBitmap(windows::fileAbsolutePathW(bitmapPath),
		(fd.spriteSheetDirection ? (fd.frameWidth * fd.frames) : fd.frameWidth),
		(fd.spriteSheetDirection ? fd.frameHeight : (fd.frameHeight * fd.frames)), &bitmap, pRT);
}

Sprite::Sprite(const char* name_, ID2D1Bitmap* bitmap_) : Sprite(name_, bitmap_, Hitbox2D::createUndefinedHitbox())
{
}

Sprite::Sprite(const char* name_, ID2D1Bitmap* bitmap_, Hitbox2D hb2d) : Sprite(name_, bitmap_, Hitbox2D::createUndefinedHitbox(), Vector2())
{
}

Sprite::Sprite(const char* name_, ID2D1Bitmap* bitmap_, Hitbox2D hb2d, Vector2 pos) : Sprite(name_, bitmap_, hb2d, pos, Rotation())
{
}

Sprite::Sprite(const char* name_, ID2D1Bitmap* bitmap_, Hitbox2D hb2d, Vector2 pos, Rotation rot) : Renderable<ID2D1Bitmap>(name_, pos, rot, bitmap_)
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