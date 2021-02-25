#pragma once

struct Rotation
{
	float x = 0.f, y = 0.f, z = 0.f; // x, y, and z rotations
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

	static Rotation add(Rotation r1, Rotation r2); // Adds 2 rotations
	static Rotation subtract(Rotation r1, Rotation r2); // Subtracts 2 rotations
	static Rotation multiply(Rotation r1, Rotation r2); // Multiplies 2 rotations
	static Rotation divide(Rotation r1, Rotation r2); // Divides 2 rotations

	static Rotation* asPtr(Rotation r);
};