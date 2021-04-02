#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "IngeniumConfig.h"
#include "Vector.h"
#include "Rotation.h"
#include "OpenGL.h"
#include "Log.h"
#include "DevIL/devil_cpp_wrapper.h"

struct Vector3D;
struct Vector2D;
struct Triangle;
struct Matrix4x4;
struct Mesh;
struct Camera;
struct Shader;
struct Material;

namespace utils3d {
	template< typename T >
	static inline std::string intToHex(T i)
	{
		std::stringstream stream;
		stream << "0x" << std::hex << i;
		return stream.str();
	}
	inline void peekGLErrors() {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			Debug::oss << "OpenGL Error: " << intToHex(err) << "\n";
		}
		Debug::write();
	};
	template <typename T> 
	inline int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
};

struct VertexArray
{
	VertexArray(float* vertPositions, int vertPositionsCount, unsigned int step = 0, unsigned int drawMode = GL_STATIC_DRAW);
	~VertexArray();
	void draw();

	unsigned int mVertexCount;
	unsigned int mVBO = GL_NONE;
	unsigned int mVAO = GL_NONE;
};

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

	float* toFloatArray();
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
		float x, u, width, r = 0;
	};
	union {
		float y, v, height, g = 0;
	};
	union {
		float w, b = 0;
	};
};

struct Triangle
{
	struct Component {
		Vector3D p; // Point
		Vector2D t; // Texture UV
		Vector3D rgb; // Colour
		Vector3D n; // Normal
	};

	// Staggered texture and vertex info for OpenGL
	Component v[3];

	static float clipAgainstPlane(Vector3D plane_p, Vector3D plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2);
};

struct Matrix4x4
{
	float m[4][4] = { 0 };

	static Matrix4x4 makeProjectionMatrix(float fovDegrees, float aspectRatio, float near_, float far_);
	static Matrix4x4 makeIdentity();
	static Matrix4x4 makeRotationX(float angleRadians);
	static Matrix4x4 makeRotationY(float angleRadians);
	static Matrix4x4 makeRotationZ(float angleRadians);

	static Matrix4x4 makeRotationAroundPoint(float xRad, float yRad, float zRad, Vector3D rotPointLocal);

	static Matrix4x4 makeTranslation(float x, float y, float z);
	static Matrix4x4 makePointedAt(Vector3D& pos, Vector3D& target, Vector3D& up);
	static Matrix4x4 makeScale(float x, float y, float z);

	Matrix4x4 qInverse ();
	void flatten(float* arr);

	friend Vector3D operator *(const Vector3D& vec, const Matrix4x4& mat);
	friend Vector3D operator *(const Matrix4x4& mat, const Vector3D& vec);

	friend Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2);
};

struct Material {
	unsigned int diffuseTex = GL_NONE;
	unsigned int specularTex = GL_NONE;
	float shininess = 0.5f;
};

struct Mesh
{
	std::vector<Triangle> tris;
	Vector3D rotation;
	Vector3D rotationCenter;
	Vector3D position;
	Vector3D scale = { 1, 1, 1 };
	Material material;

	bool loaded = false;

	unsigned int mVBO = GL_NONE; // vertex buffer object
	unsigned int mVAO = GL_NONE; // vertex array object
	unsigned int mTVBO = GL_NONE; // texture vertex buffer object

	void load();
	void setTexture(std::string texturePath, std::string specularPath = "NONE");
	void toVertexArray(VertexArray** ptr);
	Matrix4x4 makeWorldMatrix();
	void render(Shader* shader, Camera c, Matrix4x4* projectionMatrix);

	bool loadFromOBJ(std::string fileName, bool hasTexture = false, bool hasNormals = false);

private:
	static inline bool ilInitialized = false;
};

struct Camera {
	Vector3D position;
	Vector3D rotation;

	float FOV = 90.f;
	float clipNear = 0.1f;
	float clipFar = 1000.f;

	Vector3D lookVector();
	Matrix4x4 makeCameraMatrix();
};

struct Shader {
	static int compileShader(const std::string& src, unsigned int glType);
	static std::string getFileAsString(std::string path);

	unsigned int mShader;

	Shader(std::string vertexShader, std::string fragmentShader, std::string geometryShader = "none");
	void use();
	int getShaderLoc(const char* name);

	void setUniformInt(const char* name, int value);
	void setUniformBool(const char* name, bool value);
	void setUniformFloat(const char* name, float value);

	void setUniform2F(const char* name, float v1, float v2);
	void setUniform3F(const char* name, float v1, float v2, float v3);
	void setUniform4F(const char* name, float v1, float v2, float v3, float v4);

	void setUniformMatrix4x4(const char* name, Matrix4x4 mat);

	void setUniformVec4(const char* name, Vector3D v4d);
	void setUniformVec3(const char* name, Vector3D v3d);
	void setUniformVec2(const char* name, Vector2D v2d);
};