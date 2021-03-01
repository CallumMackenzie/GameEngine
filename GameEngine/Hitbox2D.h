#pragma once

#define MAX_POINTS 10

#include "Vector.h"
#include "Vector2.h"

class Hitbox2D
{
public:
	static const int TYPE_UNDEFINED = -1;
	static const int TYPE_RECTANGLE = 0;
	static const int TYPE_CIRCLE = 1;
	// static const int TYPE_POLYGON = 2;

	static Hitbox2D createUndefinedHitbox();
	static Hitbox2D createCircleHitbox(float diameter, Vector2 pos);
	static Hitbox2D createRectHitbox(Vector2 pos, Vector2 size_);

private:
	Hitbox2D(int type_);
	Hitbox2D();

public:
	Vector2 rectSize();
	Vector2 rectPos();
	Vector2 rectUL();
	Vector2 rectUR();
	Vector2 rectBR();
	Vector2 rectBL();

	float circleRadius();
	Vector2 circleCentre();

public:
	Vector2 points[MAX_POINTS];
	Vector2 position = Vector2();
	// Rotation rotation = Rotation();
	int type = TYPE_UNDEFINED;
};