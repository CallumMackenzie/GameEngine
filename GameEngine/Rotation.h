#pragma once

struct Rotation
{
	union {
		float x = 0;
		float h;
	};
	union {
		float y = 0;
		float v;
	};
	union {
		float z = 0;
		float d;
	};
	float centre [2] = {0, 0};

	Rotation(float x_, float y_, float z_); // Creates new rotation
	Rotation(); // Creates new rotation

	Rotation* add(Rotation r2); // Adds a different rotation to the current one
	Rotation* subtract(Rotation r2); // Subtracts a different rotation from the current one
	Rotation* multiply(Rotation r2); // Multiplies a different rotation with the current one
	Rotation* divide(Rotation r2); // Divides this rotation by the input one

	Rotation* add(Rotation* r2); // Adds a different rotation to the current one
	Rotation* subtract(Rotation* r2); // Subtracts a different rotation from the current one
	Rotation* multiply(Rotation* r2); // Multiplies a different rotation with the current one
	Rotation* divide(Rotation* r2); // Divides this rotation by the input one

	static float toRadians(float x);
	static float toDegrees(float x);

	static Rotation add(Rotation r1, Rotation r2); // Adds 2 rotations
	static Rotation subtract(Rotation r1, Rotation r2); // Subtracts 2 rotations
	static Rotation multiply(Rotation r1, Rotation r2); // Multiplies 2 rotations
	static Rotation divide(Rotation r1, Rotation r2); // Divides 2 rotations

	static Rotation* asPtr(Rotation r);
};