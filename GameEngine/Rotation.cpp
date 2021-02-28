#include "Rotation.h"

Rotation::Rotation(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}
Rotation::Rotation() 
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

Rotation* Rotation::add(Rotation* r2)
{
	x += r2->x;
	y += r2->y;
	z += r2->z;
	return this;
}
Rotation* Rotation::subtract(Rotation* r2)
{
	x -= r2->x;
	y -= r2->y;
	z -= r2->z;
	return this;
}
Rotation* Rotation::multiply(Rotation* r2)
{
	x *= r2->x;
	y *= r2->y;
	z *= r2->z;
	return this;
}
Rotation* Rotation::divide(Rotation* r2)
{
	x /= r2->x;
	y /= r2->y;
	z /= r2->z;
	return this;
}

float Rotation::toRadians(float x)
{
	return x * (3.14159265358979323846 / 180.0);
}

float Rotation::toDegrees(float x) 
{
	return x * (180.0 / 3.14159265358979323846);
}

Rotation* Rotation::add(Rotation r2)
{
	x += r2.x;
	y += r2.y;
	z += r2.z;
	return this;
}
Rotation* Rotation::subtract(Rotation r2)
{
	x -= r2.x;
	y -= r2.y;
	z -= r2.z;
	return this;
}
Rotation* Rotation::multiply(Rotation r2)
{
	x *= r2.x;
	y *= r2.y;
	z *= r2.z;
	return this;
}
Rotation* Rotation::divide(Rotation r2)
{
	x /= r2.x;
	y /= r2.y;
	z /= r2.z;
	return this;
}

Rotation Rotation::add(Rotation r1, Rotation r2)
{
	return Rotation(r1.x + r2.x, r1.y + r2.y, r1.z + r2.z);
}
Rotation Rotation::subtract(Rotation r1, Rotation r2)
{
	return Rotation(r1.x - r2.x, r1.y - r2.y, r1.z - r2.z);
}
Rotation Rotation::multiply(Rotation r1, Rotation r2)
{
	return Rotation(r1.x * r2.x, r1.y * r2.y, r1.z * r2.z);
}
Rotation Rotation::divide(Rotation r1, Rotation r2)
{
	return Rotation(r1.x / r2.x, r1.y / r2.y, r1.z / r2.z);
}

Rotation* Rotation::asPtr(Rotation r)
{
	return new Rotation(r.x, r.y, r.z);
}
