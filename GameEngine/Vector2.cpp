#include "Vector.h"
#include <cmath>

#include "Vector2.h"

// Callum Mackenzie

Vector2::Vector2(float x_, float y_)
{
	xVal = x_;
	yVal = y_;
}

Vector2::Vector2()
{ 

}

float Vector2::x() 
{
	return xVal;
}
float Vector2::y()
{
	return yVal;
}
float Vector2::x(float val)
{
	return (xVal = val);
}
float Vector2::y(float val)
{
	return (yVal = val);
}

float Vector2::magnitude()
{

	return 1.f / Vector::qInvSqrt(x() * x() + y() * y());
}

void Vector2::normalize()
{
	float magnitude_ = magnitude();
	xVal = xVal / magnitude_;
	yVal = yVal / magnitude_;
}


void Vector2::multiply(Vector2 v2)
{
	xVal *= v2.x();
	yVal *= v2.y();
}
void Vector2::divide(Vector2 v2)
{
	xVal /= v2.x();
	yVal /= v2.y();
}
void Vector2::add(Vector2 v2)
{
	xVal += v2.x();
	yVal += v2.y();
}
void Vector2::subtract(Vector2 v2)
{
	xVal -= v2.x();
	yVal -= v2.y();
}
void Vector2::reassign(Vector2 v2)
{
	xVal = v2.x();
	yVal = v2.y();
}

void Vector2::add(float x, float y)
{
	xVal += x;
	yVal += y;
}
void Vector2::multiply(float x, float y)
{
	xVal *= x;
	yVal *= y;
}
void Vector2::divide(float x, float y) 
{
	xVal /= x;
	yVal /= y;
}
void Vector2::subtract(float x, float y)
{
	xVal -= x;
	yVal -= y;
}

void Vector2::multiply(Vector2* v2)
{
	xVal *= v2->x();
	yVal *= v2->y();
}
void Vector2::divide(Vector2* v2)
{
	xVal /= v2->x();
	yVal /= v2->y();
}
void Vector2::add(Vector2* v2)
{
	xVal += v2->x();
	yVal += v2->y();
}
void Vector2::subtract(Vector2* v2)
{
	xVal -= v2->x();
	yVal -= v2->y();
}
void Vector2::reassign(Vector2* v2)
{
	xVal = v2->x();
	yVal = v2->y();
}

Vector2 Vector2::multiply(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x() * v2.x(), v1.y() * v2.y());
}
Vector2 Vector2::divide(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x() / v2.x(), v1.y() / v2.y());
}
Vector2 Vector2::add(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x() + v2.x(), v1.y() + v2.y());
}
Vector2 Vector2::subtract(Vector2 v1, Vector2 v2)
{
	return Vector2(v1.x() - v2.x(), v1.y() - v2.y());
}

float Vector2::hypotenuse(Vector2 point)
{
	return (1.f / Vector::qInvSqrt(point.x() * point.x() + point.y() * point.y()));
}

Vector2* Vector2::asPtr(Vector2 v)
{
	return new Vector2(v.x(), v.y());
}

