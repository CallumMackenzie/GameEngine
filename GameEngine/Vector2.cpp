#include "Vector.h"
#include <cmath>

#include "Vector2.h"

// Callum Mackenzie

Vector2::Vector2(float x_, float y_)
{
	x = x_;
	y = y_;
}

Vector2::Vector2()
{ 
	x = 0;
	y = 0;
}

float Vector2::getX()
{
	return x;
}
float Vector2::getY()
{
	return y;
}
float Vector2::setX(float v)
{
	return x = v;
}
float Vector2::setY(float v)
{
	return y = v;
}

float Vector2::magnitude()
{

	return Vector::qSqrt(x * x + y * y);
}

void Vector2::normalize()
{
	float magnitude_ = magnitude();
	x = x / magnitude_;
	y = y / magnitude_;
}


void Vector2::multiply(Vector2 v2)
{
	x *= v2.x;
	y *= v2.y;
}
void Vector2::divide(Vector2 v2)
{
	x /= v2.x;
	y /= v2.y;
}
void Vector2::add(Vector2 v2)
{
	x += v2.x;
	y += v2.y;
}
void Vector2::subtract(Vector2 v2)
{
	x -= v2.x;
	y -= v2.y;
}
void Vector2::reassign(Vector2 v2)
{
	x = v2.x;
	y = v2.y;
}

void Vector2::add(float x, float y)
{
	x += x;
	y += y;
}
void Vector2::multiply(float x, float y)
{
	x *= x;
	y *= y;
}
void Vector2::divide(float x, float y) 
{
	x /= x;
	y /= y;
}
void Vector2::subtract(float x, float y)
{
	x -= x;
	y -= y;
}

void Vector2::multiply(Vector2* v2)
{
	x *= v2->x;
	y *= v2->y;
}
void Vector2::divide(Vector2* v2)
{
	x /= v2->x;
	y /= v2->y;
}
void Vector2::add(Vector2* v2)
{
	x += v2->x;
	y += v2->y;
}
void Vector2::subtract(Vector2* v2)
{
	x -= v2->x;
	y -= v2->y;
}
void Vector2::reassign(Vector2* v2)
{
	x = v2->x;
	y = v2->y;
}

void Vector2::rotate(float thetaRadians)
{
	x = std::cos(thetaRadians) * x - std::sin(thetaRadians) * y;
	y = std::sin(thetaRadians) * x + std::cos(thetaRadians) * y;
}

float Vector2::dotProduct(Vector2 v2)
{
	return x * v2.x + y * v2.y;
}

Vector2 Vector2::multiply(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x * v2.x, v1.y * v2.y);
}
Vector2 Vector2::divide(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x / v2.x, v1.y / v2.y);
}
Vector2 Vector2::add(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}
Vector2 Vector2::subtract(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2* Vector2::multiply(Vector2* v1, Vector2* v2)
{
	return new Vector2(v1->x * v2->x, v1->y * v2->y);
}

Vector2* Vector2::divide(Vector2* v1, Vector2* v2)
{
	return new Vector2(v1->x / v2->x, v1->y / v2->y);
}

Vector2* Vector2::add(Vector2* v1, Vector2* v2)
{
	return new Vector2(v1->x + v2->x, v1->y + v2->y);
}

Vector2* Vector2::subtract(Vector2* v1, Vector2* v2)
{
	return new Vector2(v1->x - v2->x, v1->y - v2->y);
}

float Vector2::hypotenuse(Vector2 point)
{
	return (Vector::qSqrt(point.x * point.x + point.y * point.y));
}

Vector2* Vector2::asPtr(Vector2 v)
{
	return new Vector2(v.x, v.y);
}

