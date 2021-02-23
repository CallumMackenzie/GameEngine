#include "Vector.h"

#include "Vector2.h"

// Callum Mackenzie

Vector2::Vector2(float x_, float y_) : Vector(new float[2]{ x_, y_ }, 2)  
{ }

Vector2::Vector2() : Vector(new float[2]{ 0.f, 0.f }, 2)
{ }

float Vector2::x() 
{
	return vector[0];
}
float Vector2::y()
{
	return vector[1];
}
float Vector2::x(float val)
{
	return (vector[0] = val);
}
float Vector2::y(float val)
{
	return (vector[1] = val);
}

float Vector2::magnitude()
{

	return 1.f / qInvSqrt(x() * x() + y() * y());
}
Vector2* Vector2::normalize()
{
	float magnitude_ = magnitude();
	vector[0] = vector[0] / magnitude_;
	vector[1] = vector[1] / magnitude_;
	return this;
}

Vector2* Vector2::multiply(Vector2 v2)
{
	vector[0] *= v2.x();
	vector[1] *= v2.y();
	return this;
}
Vector2* Vector2::divide(Vector2 v2)
{
	vector[0] /= v2.x();
	vector[1] /= v2.y();
	return this;
}
Vector2* Vector2::add(Vector2 v2)
{
	vector[0] += v2.x();
	vector[1] += v2.y();
	return this;
}
Vector2* Vector2::subtract(Vector2 v2)
{
	vector[0] -= v2.x();
	vector[1] -= v2.y();
	return this;
}
Vector2* Vector2::reassign(Vector2 v2)
{
	vector[0] = v2.x();
	vector[1] = v2.y();
	return this;
}

Vector2* Vector2::multiply(Vector2* v2)
{
	vector[0] *= v2->x();
	vector[1] *= v2->y();
	return this;
}
Vector2* Vector2::divide(Vector2* v2)
{
	vector[0] /= v2->x();
	vector[1] /= v2->y();
	return this;
}
Vector2* Vector2::add(Vector2* v2)
{
	vector[0] += v2->x();
	vector[1] += v2->y();
	return this;
}
Vector2* Vector2::subtract(Vector2* v2)
{
	vector[0] -= v2->x();
	vector[1] -= v2->y();
	return this;
}
Vector2* Vector2::reassign(Vector2* v2)
{
	vector[0] = v2->x();
	vector[1] = v2->y();
	return this;
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

Vector2* Vector2::asPtr(Vector2 v)
{
	return new Vector2(v.x(), v.y());
}

