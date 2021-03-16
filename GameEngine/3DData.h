#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "IngeniumConfig.h"
#include "Vector.h"
#include "Rotation.h"

struct Vector3D;
struct Vector2D;
struct Triangle;
struct Matrix4x4;
struct Mesh;
struct Camera;

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
	float w = 1;

	static Vector3D planeIntersect (Vector3D& plane_p, Vector3D& plane_n, Vector3D& lineStart, Vector3D& lineEnd, float& t);
	static float dotProduct(Vector3D& v1, Vector3D& v2);
	static Vector3D crossProduct(Vector3D& v1, Vector3D& v2);

	float length();
	void normalize();
	Vector3D normalized();

	friend Vector3D operator *(const Vector3D& vec, const Matrix4x4& mat);
	friend Vector3D operator *(const Matrix4x4& mat, const Vector3D& vec);

	friend Vector3D operator *(const Vector3D& v1, const float k);
	friend Vector3D operator /(const Vector3D& v1, const float k);

	friend Vector3D operator *(const Vector3D& v1, const Vector3D& v2);
	friend Vector3D operator /(const Vector3D& v1, const Vector3D& v2);
	friend Vector3D operator +(const Vector3D& v1, const Vector3D& v2);
	friend Vector3D operator -(const Vector3D& v1, const Vector3D& v2);
};

struct Vector2D
{
	union {
		float x, u, width = 0;
	};
	union {
		float y, v, height = 0;
	};
	float w = 0;
};

struct Triangle
{
	Vector3D p[3] = { 0 }; // Points
	Vector2D t[3] = { 0 }; // Texture
	long col = 0x000000;

	static float clipAgainstPlane(Vector3D plane_p, Vector3D plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2);
};

struct Matrix4x4
{
	float m[4][4] = { 0 };

	static Matrix4x4 makeProjectionMatrix(float fovDegrees, float aspectRatio, float near, float far);
	static Matrix4x4 makeIdentity();
	static Matrix4x4 makeRotationX(float angleRadians);
	static Matrix4x4 makeRotationY(float angleRadians);
	static Matrix4x4 makeRotationZ(float angleRadians);
	static Matrix4x4 makeTranslation(float x, float y, float z);
	static Matrix4x4 makePointedAt(Vector3D& pos, Vector3D& target, Vector3D& up);

	Matrix4x4 qInverse ();

	friend Vector3D operator *(const Vector3D& vec, const Matrix4x4& mat);
	friend Vector3D operator *(const Matrix4x4& mat, const Vector3D& vec);

	friend Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2);
};

struct Mesh
{
	std::vector<Triangle> tris;
	Vector3D rotation;
	Vector3D position;
	float scale = 1;

	bool loadFromOBJ(std::string fileName, bool hasTexture = false);
};

struct Camera {
	Vector3D position;
	Vector3D rotation;

	float FOV = 90.f;
	float clipNear = 0.1f;
	float clipFar = 1000.f;

	Vector3D lookVector();
};