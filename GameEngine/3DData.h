#pragma once

#include "Vector.h"
#include "Rotation.h"
#include <vector>

struct Vector3D;
struct Vector2D;
struct Triangle;
struct Matrix4x4;
struct Mesh;

struct Vector3D
{
	union {
		float x, width = 0;
	};
	union {
		float y, height = 0;
	};
	union {
		float z, depth = 0;
	};
	float w = 0;

	static float dotProduct(Vector3D& v1, Vector3D& v2);
	static Vector3D crossProduct(Vector3D& v1, Vector3D& v2);

	float length();
	void normalize();
	Vector3D normalized();

	friend Vector3D operator *(const Vector3D& vec, const Matrix4x4& mat);

	friend Vector3D operator *(const Vector3D& v1, const float k);
	friend Vector3D operator /(const Vector3D& v1, const float k);

	friend Vector3D operator +(const Vector3D& v1, const Vector3D& v2);
	friend Vector3D operator -(const Vector3D& v1, const Vector3D& v2);
};

struct Triangle
{
	union {
		Vector3D p[3];
		Vector3D points[3];
	};
};

struct Vector2D
{
	union {
		float x, width = 0;
	};
	union {
		float y, height = 0;
	};
	float w = 0;
};

struct Matrix4x4
{
	float m[4][4] = { 0 };

	Matrix4x4 makeIdentity();
	Matrix4x4 rotatedX(float angleRadians);
	Matrix4x4 rotatedY(float angleRadians);
	Matrix4x4 rotatedZ(float angleRadians);
	Matrix4x4 translated(float x, float y, float z);
	Matrix4x4 projected(float fovDegrees, float aspectRatio, float near, float far);

	friend Vector3D operator *(const Vector3D& vec, const Matrix4x4& mat);

	friend Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2);
};

struct Mesh
{
	std::vector<Triangle> tris;
};