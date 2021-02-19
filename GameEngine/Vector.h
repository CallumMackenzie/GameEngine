#pragma once

// TODO: Add, subtract, multiply, divide, normalize, get magnitude of multi-dimensional vector

struct Vector
{
	Vector(float* arr, int length_);
	static float qInvSqrt(float value); // Fast inverse square root with 2 passes
	static float qInvSqrt(float value, int passes); // Fast inverse square root with 2 passes
	float* vector;
	int length = 1;
};