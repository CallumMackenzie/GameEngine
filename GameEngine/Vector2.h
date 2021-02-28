#pragma once

#include "Rotation.h"

// Callum Mackenzie

struct Vector2
{
	float xVal = 0;
	float yVal = 0;

	float x(); // Returns the x component of the vector
	float y(); // Returns the y component of the vector

	float x(float val); // Sets the y component of the vector
	float y(float val); // Sets the y component of the vector

	Vector2(float x_, float y_); // Creates new vector2
	Vector2(); // Creates new vector2 with magnitude 0
	float magnitude(); // Returns the magnitude of this vector
	void normalize(); // Normalizes this vector

	void multiply(Vector2 v2); // Multiplies the vector with the vector passed in
	void divide(Vector2 v2); // Divides this vector by the passed vector
	void add(Vector2 v2); // Adds this vector to the passed vector
	void subtract(Vector2 v2); // Subtracts the passed vector from this vector
	void reassign(Vector2 v2); // Reassigns this vector to the input one

	void add(float x, float y);
	void multiply(float x, float y);
	void divide(float x, float y);
	void subtract(float x, float y);

	void multiply(Vector2* v2); // Multiplies the vector with the vector passed in
	void divide(Vector2* v2); // Divides this vector by the passed vector
	void add(Vector2* v2); // Adds this vector to the passed vector
	void subtract(Vector2* v2); // Subtracts the passed vector from this vector
	void reassign(Vector2* v2); // Reassigns this vector to the input one

	static Vector2 multiply(Vector2 v1, Vector2 v2); // Multiplies 2 vector2s
	static Vector2 divide(Vector2 v1, Vector2 v2); // Divides the right by the left
	static Vector2 add(Vector2 v1, Vector2 v2); // Adds 2 vectors
	static Vector2 subtract(Vector2 v1, Vector2 v2); // Subtracts the right from the left

	static float hypotenuse(Vector2 point);

	static Vector2* asPtr(Vector2 v);
};