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

void Rotation::add(Rotation* r2)
{
	x += r2->x;
	y += r2->y;
	z += r2->z;
}

void Rotation::subtract(Rotation* r2)
{
	x -= r2->x;
	y -= r2->y;
	z -= r2->z;
}

void Rotation::multiply(Rotation* r2)
{
	x *= r2->x;
	y *= r2->y;
	z *= r2->z;
}

void Rotation::divide(Rotation* r2)
{
	x /= r2->x;
	y /= r2->y;
	z /= r2->z;
}

Rotation* Rotation::add(Rotation* r1, Rotation* r2)
{
	return new Rotation(r1->x + r2->x, r1->y + r2->y, r1->z + r2->z);
}

Rotation* Rotation::subtract(Rotation* r1, Rotation* r2)
{
	return new Rotation(r1->x - r2->x, r1->y - r2->y, r1->z - r2->z);
}

Rotation* Rotation::multiply(Rotation* r1, Rotation* r2)
{
	return new Rotation(r1->x * r2->x, r1->y * r2->y, r1->z * r2->z);
}

Rotation* Rotation::divide(Rotation* r1, Rotation* r2)
{
	return new Rotation(r1->x / r2->x, r1->y / r2->y, r1->z / r2->z);
}
