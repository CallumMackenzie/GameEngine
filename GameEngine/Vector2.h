#pragma once

// Callum Mackenzie

struct Vector2 : public Vector
{
	float x(); // Returns the x component of the vector
	float y(); // Returns the y component of the vector

	float x(float val); // Sets the y component of the vector
	float y(float val); // Sets the y component of the vector

	Vector2(float x_, float y_); // Creates new vector2
	Vector2(); // Creates new vector2 with magnitude 0
	float magnitude(); // Returns the magnitude of this vector
	Vector2* normalize(); // Normalizes this vector

	Vector2* multiply(Vector2 v2); // Multiplies the vector with the vector passed in
	Vector2* divide(Vector2 v2); // Divides this vector by the passed vector
	Vector2* add(Vector2 v2); // Adds this vector to the passed vector
	Vector2* subtract(Vector2 v2); // Subtracts the passed vector from this vector
	Vector2* reassign(Vector2 v2); // Reassigns this vector to the input one

	Vector2* multiply(Vector2* v2); // Multiplies the vector with the vector passed in
	Vector2* divide(Vector2* v2); // Divides this vector by the passed vector
	Vector2* add(Vector2* v2); // Adds this vector to the passed vector
	Vector2* subtract(Vector2* v2); // Subtracts the passed vector from this vector
	Vector2* reassign(Vector2* v2); // Reassigns this vector to the input one

	static Vector2 multiply(Vector2 v1, Vector2 v2); // Multiplies 2 vector2s
	static Vector2 divide(Vector2 v1, Vector2 v2); // Divides 2 vectors
	static Vector2 add(Vector2 v1, Vector2 v2); // Adds 2 vectors
	static Vector2 subtract(Vector2 v1, Vector2 v2); // Subtracts 2 vectors

	static Vector2* asPtr(Vector2 v);
};