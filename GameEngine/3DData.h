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
#include "Cache.h"
#include "DevIL/devil_cpp_wrapper.h"

struct Vec3;
struct Vec2;
struct Triangle;
struct Mat4;
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
	inline float fastReciprocal(float x) {
		union {
			float flt[2];
			std::uint_least32_t ull[2];
		} u;
		u.flt[0] = x; 
		u.ull[1] = 0;
		u.ull[0] = (0xbe6eb3beU - u.ull[0]) >> 1;
		u.flt[1] = 0;
		u.flt[0] *= u.flt[0];
		return u.flt[0];
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

struct Vec3
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

	static Vec3 planeIntersect(Vec3& plane_p, Vec3& plane_n, Vec3& lineStart, Vec3& lineEnd, float& t);
	static float dotProduct(Vec3& v1, Vec3& v2);
	static Vec3 crossProduct(Vec3& v1, Vec3& v2);

	float* toFloatArray();
	float length();
	void normalize();
	Vec3 normalized();

	friend Vec3 operator *(const Vec3& vec, const Mat4& mat);
	friend Vec3 operator *(const Mat4& mat, const Vec3& vec);

	friend Vec3 operator *(const Vec3& v1, const float k);
	friend Vec3 operator /(const Vec3& v1, const float k);

	friend Vec3 operator *(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator /(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator +(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator -(const Vec3& v1, const Vec3& v2);
};

struct Vec2
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
	friend Vec2 operator +(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator -(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator *(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator /(const Vec2& v1, const Vec2& v2);
};

struct Triangle
{
	struct Component {
		Vec3 p; // Point
		Vec2 t; // Texture UV
		Vec3 rgb; // Colour
		Vec3 n; // Normal
		Vec3 tan; // Tangent
	};

	// Staggered texture and vertex info for OpenGL
	Component v[3];
};

struct Mat4
{
	float m[4][4] = { 0 };

	static Mat4 makeProjectionMatrix(float fovDegrees, float aspectRatio, float near_, float far_);
	static Mat4 makeIdentity();
	static Mat4 makeRotationX(float angleRadians);
	static Mat4 makeRotationY(float angleRadians);
	static Mat4 makeRotationZ(float angleRadians);

	static Mat4 makeRotationAroundPoint(float xRad, float yRad, float zRad, Vec3 rotPointLocal);

	static Mat4 makeTranslation(float x, float y, float z);
	static Mat4 makePointedAt(Vec3& pos, Vec3& target, Vec3& up);
	static Mat4 makeScale(float x, float y, float z);

	Mat4 qInverse();
	void flatten(float* arr);

	friend Vec3 operator *(const Vec3& vec, const Mat4& mat);
	friend Vec3 operator *(const Mat4& mat, const Vec3& vec);

	friend Mat4 operator *(const Mat4& m1, const Mat4& m2);
};

struct Material {
	unsigned int diffuseTex = GL_NONE;
	unsigned int specularTex = GL_NONE;
	unsigned int normalTex = GL_NONE;
	float shininess = 0.5f;
};

struct Mesh
{
	static inline Cache<std::string, int> textureReferenceCache = Cache<std::string, int>();
	static inline Cache<std::string, unsigned int*> geometryReferenceCache = Cache<std::string, unsigned int*>();
	static inline Cache<std::string, std::vector<Triangle>> geometryValueCache = Cache<std::string, std::vector<Triangle>>();

	Vec3 position;
	Vec3 rotation;
	Vec3 scale = { 1, 1, 1 };
	Vec3 rotationCenter;
	Material material;
	std::vector<Triangle> tris;
	unsigned int numTris = 0;

	static unsigned int loadTexture(std::string path, unsigned int texSlot, unsigned int sWrap = GL_REPEAT, unsigned int tWrap = GL_REPEAT, unsigned int minFilter = GL_LINEAR_MIPMAP_LINEAR,
		unsigned int magFilter = GL_LINEAR);
	static void renderAll(Shader* shader, Camera c, Mat4* projectionMatrix, std::vector<Mesh> m);

	bool loaded = false;

	unsigned int mVBO = GL_NONE; // vertex buffer object
	unsigned int mVAO = GL_NONE; // vertex array object
	unsigned int mTVBO = GL_NONE; // texture vertex buffer object

	void make(std::string obj, std::string texturePath = "NONE", std::string specularPath = "NONE", std::string normalPath = "NONE");
	void load();
	void setTexture(std::string texturePath, std::string specularPath = "NONE", std::string normalPath = "NONE");
	void toVertexArray(VertexArray** ptr);
	Mat4 makeWorldMatrix();
	void render(Shader* shader, Camera c, Mat4* projectionMatrix);

	bool loadFromOBJ(std::string fileName);

private:
	static inline bool ilInitialized = false;
	static Vec3 calcTangent(Triangle tri);
};

struct Camera {
	Vec3 position;
	Vec3 rotation;

	float FOV = 90.f;
	float clipNear = 0.1f;
	float clipFar = 1000.f;

	Vec3 lookVector();
	Mat4 makeCameraMatrix();
};

struct Shader {
	static int compileShader(const std::string& src, unsigned int glType);
	static std::string getFileAsString(std::string path);

	unsigned int mShader;

	Shader(std::string vertexShader, std::string fragmentShader, std::string geometryShader = "NONE");
	void use();
	int getShaderLoc(const char* name);

	void setUniformInt(const char* name, int value);
	void setUniformBool(const char* name, bool value);
	void setUniformFloat(const char* name, float value);

	void setUniform2F(const char* name, float v1, float v2);
	void setUniform3F(const char* name, float v1, float v2, float v3);
	void setUniform4F(const char* name, float v1, float v2, float v3, float v4);

	void setUniformMat4(const char* name, Mat4 mat);

	void setUniformVec4(const char* name, Vec3 v4d);
	void setUniformVec3(const char* name, Vec3 v3d);
	void setUniformVec2(const char* name, Vec2 v2d);
};