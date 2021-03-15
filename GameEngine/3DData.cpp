#include "3DData.h"

Vector3D operator*(const Vector3D& i, const Matrix4x4& m)
{
	Vector3D v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}
Vector3D operator*(const Vector3D& m1, const float k)
{
	return { m1.x * k, m1.y * k, m1.z * k };
}
Vector3D operator/(const Vector3D& m1, const float k)
{
	return { m1.x / k, m1.y / k, m1.z / k };
}
Vector3D operator+(const Vector3D& m1, const Vector3D& m2)
{
	return { m1.x + m2.x, m1.y + m2.y, m1.z + m2.z };
}
Vector3D operator-(const Vector3D& m1, const Vector3D& m2)
{
	return { m1.x - m2.x, m1.y - m2.y, m1.z - m2.z };
}
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}

float Vector3D::dotProduct(Vector3D& v1, Vector3D& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vector3D Vector3D::crossProduct(Vector3D& v1, Vector3D& v2)
{
	Vector3D v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}
float Vector3D::length()
{
	return Vector::qSqrt(dotProduct(*this, *this), 1);
}
void Vector3D::normalize()
{
	float l = length();
	x /= l;
	y /= l;
	z /= l;
}
Vector3D Vector3D::normalized()
{
	float l = length();
	return { x / l, y / l, z / l };
}

Matrix4x4 Matrix4x4::makeIdentity()
{
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Matrix4x4 Matrix4x4::rotatedX(float xAngleRadians)
{
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(xAngleRadians);
	matrix.m[1][2] = sinf(xAngleRadians);
	matrix.m[2][1] = -sinf(xAngleRadians);
	matrix.m[2][2] = cosf(xAngleRadians);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Matrix4x4 Matrix4x4::rotatedY(float yAngleRadians)
{
	Matrix4x4 matrix;
	matrix.m[0][0] = cosf(yAngleRadians);
	matrix.m[0][2] = sinf(yAngleRadians);
	matrix.m[2][0] = -sinf(yAngleRadians);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(yAngleRadians);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Matrix4x4 Matrix4x4::rotatedZ(float zAngleRadians)
{
	Matrix4x4 matrix;
	matrix.m[0][0] = cosf(zAngleRadians);
	matrix.m[0][1] = sinf(zAngleRadians);
	matrix.m[1][0] = -sinf(zAngleRadians);
	matrix.m[1][1] = cosf(zAngleRadians);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Matrix4x4 Matrix4x4::translated(float x, float y, float z)
{
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}
Matrix4x4 Matrix4x4::projected(float fovDegrees, float aspectRatio, float near, float far)
{
	float fovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * 3.14159f);
	Matrix4x4 matrix;
	matrix.m[0][0] = aspectRatio * fovRad;
	matrix.m[1][1] = fovRad;
	matrix.m[2][2] = far / (far - near);
	matrix.m[3][2] = (-far * near) / (far - near);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}