#include "3DData.h"

Vec3 operator*(const Vec3& i, const Mat4& m)
{
	Vec3 v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}
Vec3 operator*(const Mat4& mat, const Vec3& vec)
{
	return vec * mat;
}
Vec3 operator*(const Vec3& m1, const float k)
{
	return { m1.x * k, m1.y * k, m1.z * k };
}
Vec3 operator/(const Vec3& m1, const float k)
{
	return { m1.x / k, m1.y / k, m1.z / k };
}
Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	return { v1.x * v2.x, v1.y + v2.y, v1.z * v2.z, v1.w * v2.w };
}
Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
	return { v1.x / v2.x, v1.y + v2.y, v1.z / v2.z, v1.w / v2.w };
}
Vec3 operator+(const Vec3& m1, const Vec3& m2)
{
	return { m1.x + m2.x, m1.y + m2.y, m1.z + m2.z };
}
Vec3 operator-(const Vec3& m1, const Vec3& m2)
{
	return { m1.x - m2.x, m1.y - m2.y, m1.z - m2.z };
}
Mat4 operator*(const Mat4& m1, const Mat4& m2)
{
	Mat4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}
Vec2 operator -(const Vec2& v1, const Vec2& v2) {
	return { v1.x - v2.x, v1.y - v2.y };
};
Vec2 operator +(const Vec2& v1, const Vec2& v2) {
	return { v1.x + v2.x, v1.y + v2.y };
};
Vec2 operator *(const Vec2& v1, const Vec2& v2) {
	return { v1.x * v2.x, v1.y * v2.y };
};
Vec2 operator /(const Vec2& v1, const Vec2& v2) {
	return { v1.x / v2.x, v1.y / v2.y };
};
Vec3 Vec3::planeIntersect(Vec3& plane_p, Vec3& plane_n, Vec3& lineStart, Vec3& lineEnd, float& t)
{
	plane_n = plane_n.normalized();
	float plane_d = -Vec3::dotProduct(plane_n, plane_p);
	float ad = Vec3::dotProduct(lineStart, plane_n);
	float bd = Vec3::dotProduct(lineEnd, plane_n);
	t = (-plane_d - ad) / (bd - ad);
	Vec3 lineStartToEnd = lineEnd - lineStart;
	Vec3 lineToIntersect = lineStartToEnd * t;
	return lineStart + lineToIntersect;
}
float Vec3::dotProduct(Vec3& v1, Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vec3 Vec3::crossProduct(Vec3& v1, Vec3& v2)
{
	Vec3 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}
float Vec3::length()
{
	return Vector::qSqrt(dotProduct(*this, *this), 1);
}
float* Vec3::toFloatArray()
{
	float* arr = new float[4];
	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
	arr[3] = w;
	return arr;
}
void Vec3::normalize()
{
	float l = length();
	x /= l;
	y /= l;
	z /= l;
}
Vec3 Vec3::normalized()
{
	float l = length();
	return { x / l, y / l, z / l };
}
Mat4 Mat4::makeIdentity()
{
	Mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Mat4 Mat4::makeRotationX(float xAngleRadians)
{
	Mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(xAngleRadians);
	matrix.m[1][2] = sinf(xAngleRadians);
	matrix.m[2][1] = -sinf(xAngleRadians);
	matrix.m[2][2] = cosf(xAngleRadians);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Mat4 Mat4::makeRotationY(float yAngleRadians)
{
	Mat4 matrix;
	matrix.m[0][0] = cosf(yAngleRadians);
	matrix.m[0][2] = sinf(yAngleRadians);
	matrix.m[2][0] = -sinf(yAngleRadians);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(yAngleRadians);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Mat4 Mat4::makeRotationZ(float zAngleRadians)
{
	Mat4 matrix;
	matrix.m[0][0] = cosf(zAngleRadians);
	matrix.m[0][1] = sinf(zAngleRadians);
	matrix.m[1][0] = -sinf(zAngleRadians);
	matrix.m[1][1] = cosf(zAngleRadians);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
Mat4 Mat4::makeTranslation(float x, float y, float z)
{
	Mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}
Mat4 Mat4::makeProjectionMatrix(float fovDegrees, float aspectRatio, float near_, float far_)
{
	float fovRad = 1.0f / tanf(Rotation::toRadians(fovDegrees * 0.5f));
	Mat4 matrix;
	matrix.m[0][0] = aspectRatio * fovRad;
	matrix.m[1][1] = fovRad;
	matrix.m[2][2] = far_ / (far_ - near_);
	matrix.m[3][2] = (-far_ * near_) / (far_ - near_);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}
Mat4 Mat4::makePointedAt(Vec3& pos, Vec3& target, Vec3& up)
{
	Vec3 newForward = target - pos;
	newForward = newForward.normalized();

	Vec3 a = newForward * Vec3::dotProduct(up, newForward);
	Vec3 newUp = up - a;
	newUp = newUp.normalized();

	Vec3 newRight = Vec3::crossProduct(newUp, newForward);

	Mat4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;
}
Mat4 Mat4::makeScale(float x, float y, float z)
{
	Mat4 matrix = makeIdentity();
	matrix.m[0][0] = x;
	matrix.m[1][1] = y;
	matrix.m[2][2] = z;
	return matrix;
}
Mat4 Mat4::qInverse()
{

	Mat4 matrix;
	matrix.m[0][0] = m[0][0]; matrix.m[0][1] = m[1][0]; matrix.m[0][2] = m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m[0][1]; matrix.m[1][1] = m[1][1]; matrix.m[1][2] = m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m[0][2]; matrix.m[2][1] = m[1][2]; matrix.m[2][2] = m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m[3][0] * matrix.m[0][0] + m[3][1] * matrix.m[1][0] + m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m[3][0] * matrix.m[0][1] + m[3][1] * matrix.m[1][1] + m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m[3][0] * matrix.m[0][2] + m[3][1] * matrix.m[1][2] + m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
void Mat4::flatten(float* arr)
{
	arr = new float[16];
	int k = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			arr[k] = m[i][j];
			k++;
		}
}
Mat4 Mat4::makeRotationAroundPoint(float xRad, float yRad, float zRad, Vec3 rotPoint)
{
	Mat4 mat = (
		makeTranslation(rotPoint.x, rotPoint.y, rotPoint.z) *
		makeRotationX(xRad) * makeRotationY(yRad) * makeRotationZ(zRad)) *
		makeTranslation(-rotPoint.x, -rotPoint.y, -rotPoint.z);
	return mat;
}
bool Mesh::loadFromOBJ(std::string fileName)
{
	bool debugLoadTime = true;
	auto startLoad = clock();
	if (geometryValueCache.use && useGeometryValueCache)
		if (geometryValueCache.containsKey(fileName)) {
			tris = geometryValueCache.cache[fileName];
			numTris = tris.size();
			if (debugLoadTime) {
				Debug::oss << "Geometry value cache hit (" << fileName << "): " << (((float)(clock() - startLoad) / (float)CLOCKS_PER_SEC) * 1000.f) << "ms";
				Debug::writeLn();
			}
			return true;
		}
	if (geometryReferenceCache.use && useGeometryReferenceCache)
		if (geometryReferenceCache.containsKey(fileName)) {
			mVBO = geometryReferenceCache.cache[fileName][0];
			mVAO = geometryReferenceCache.cache[fileName][1];
			numTris = geometryReferenceCache.cache[fileName][2];
			if (debugLoadTime) {
				Debug::oss << "Geometry reference cache hit (" << fileName << "): " << (((float)(clock() - startLoad) / (float)CLOCKS_PER_SEC) * 1000.f) << "ms";
				Debug::writeLn();
			}
			loaded = true;
			return true;
		}
	bool hasTexture = false;
	bool hasNormals = false;

	using namespace std;
	ifstream f(fileName);
	if (!f.is_open())
		return false;

	vector<Vec3> verts;
	vector<Vec3> normals;
	vector<Vec2> texs;

	while (!f.eof()) {
		char line[MAX_OBJ_LINE_CHARS];
		char junk;
		f.getline(line, MAX_OBJ_LINE_CHARS);
		stringstream s;

		s << line;
		if (line[0] == 'v') {
			if (line[1] == 't') {
				hasTexture = true;
				Vec2 v;
				s >> junk >> junk >> v.u >> v.v;
				texs.push_back(v);
			}
			else if (line[1] == 'n') {
				hasNormals = true;
				Vec3 normal;
				s >> junk >> junk >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else {
				Vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}
		if (!hasTexture && !hasNormals) {
			if (line[0] == 'f') {
				int face[3] = { 0 };
				s >> junk >> face[0] >> face[1] >> face[2];
				Triangle fTri;
				for (int j = 0; j < 3; j++)
					fTri.v[j].p = verts[face[j] - 1];
				tris.push_back(fTri);
			}
		}
		else {
			if (line[0] == 'f') {
				s >> junk;

				string* tokens;
				int params = 2;
				if (hasNormals) {
					tokens = new string[9];
					params = 3;
				}
				else {
					tokens = new string[6];
				}
				int nTokenCount = -1;

				while (!s.eof())
				{
					char c = s.get();
					if (c == ' ' || c == '/')
						nTokenCount++;
					else
						tokens[nTokenCount].append(1, c);
				}

				tokens[nTokenCount].pop_back();

				Triangle push;
				for (int k = 0; k < 3; k++)
				{
					push.v[k].p = verts[stoi(tokens[params * k]) - 1];
					push.v[k].t = texs[stoi(tokens[1 + params * k]) - 1];
					if (hasNormals)
						push.v[k].n = normals[stoi(tokens[2 + params * k]) - 1];
				}

				Vec3 tangent = calcTangent(push);
				for (int i = 0; i < 3; i++) push.v[i].tan = tangent;
				tris.push_back(push);
				delete[] tokens;
			}
		}
	}
	numTris = tris.size();
	if (debugLoadTime) {
		Debug::oss << "No cache hits (" << fileName << "): " << (((float)(clock() - startLoad) / (float)CLOCKS_PER_SEC) * 1000.f) << "ms";
		Debug::writeLn();
	}
	if (geometryValueCache.use && useGeometryValueCache)
		if (!geometryValueCache.containsKey(fileName))
			geometryValueCache.add(fileName, tris);
	return true;
}
Mat4 Mesh::makeWorldMatrix()
{
	Mat4 matRot = Mat4::makeRotationAroundPoint(rotation.x, rotation.y, rotation.z, rotationCenter);
	Mat4 matTrans = Mat4::makeTranslation(position.x, position.y, position.z);
	Mat4 matScale = Mat4::makeScale(scale.x, scale.y, scale.z);
	Mat4 matWorld = (matScale * matRot * matTrans);
	return matWorld;
}
void Mesh::toVertexArray(VertexArray** ptr)
{
	std::vector<float> data;

	for (int i = 0; i < tris.size(); i++) {
		float* arr = tris[i].v[0].p.toFloatArray();
		for (int k = 0; k < 3; k++)
			data.push_back(arr[k]);
		delete arr;
		arr = tris[i].v[1].p.toFloatArray();
		for (int k = 0; k < 3; k++)
			data.push_back(arr[k]);
		delete arr;
		arr = tris[i].v[2].p.toFloatArray();
		for (int k = 0; k < 3; k++)
			data.push_back(arr[k]);
		delete arr;
	}

	*ptr = new VertexArray(data.data(), tris.size() * 9, 0, GL_STATIC_DRAW);
}
void Mesh::load() {
	if (!loaded) {
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		glBufferData(GL_ARRAY_BUFFER, tris.size() * sizeof(Triangle), &tris[0], GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Component), NULL);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle::Component), (void*)(sizeof(Vec3)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Component), (void*)(sizeof(Vec3) + sizeof(Vec2)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Component), (void*)(sizeof(Vec3) + sizeof(Vec2) + sizeof(Vec3)));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Component), (void*)(sizeof(Vec3) + sizeof(Vec2) + sizeof(Vec3) + sizeof(Vec3)));
		glEnableVertexAttribArray(4);
	}
	loaded = true;
}
unsigned int Mesh::loadTexture(std::string path, unsigned int texSlot, bool useRefCache, unsigned int sWrap, unsigned int tWrap, unsigned int minFilter, unsigned int magFilter)
{
	bool debugCache = false;
	auto startLoad = clock();
	if (textureReferenceCache.use && useRefCache)
		if (textureReferenceCache.containsKey(path)) {
			if (debugCache) {
				Debug::oss << "Texture reference cache hit (" << path << "): " << (((float)(clock() - startLoad) / (float)CLOCKS_PER_SEC) * 1000.f) << "ms";
				Debug::writeLn();
			}
			return textureReferenceCache.cache[path];
		}

	unsigned int tex = GL_NONE;
	auto mode = GL_RGBA;
	auto ilMode = IL_RGBA;
	glGenTextures(1, &tex);
	glActiveTexture(texSlot);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	float px[] = { 0.f, 0.f, 0.f, 1.f };

	glTexImage2D(GL_TEXTURE_2D, 0, mode, 0, 0, 0, mode, GL_UNSIGNED_BYTE, px);
	if (!ilInitialized) {
		ilInit();
		iluInit();
		ilClearColour(255, 255, 255, 000);
		ilInitialized = true;
	}
	if (path != "NONE") {
		ILuint imgID = 0;
		ilGenImages(1, &imgID);
		ilBindImage(imgID);
		ILboolean success = ilLoadImage(path.c_str());
		if (success == IL_TRUE)
		{
			success = ilConvertImage(ilMode, IL_UNSIGNED_BYTE);
			if (success == IL_TRUE) {
				unsigned int* data = (unsigned int*)ilGetData();
				unsigned int width = (unsigned int)ilGetInteger(IL_IMAGE_WIDTH), height = (unsigned int)ilGetInteger(IL_IMAGE_HEIGHT);

				if (data) {
					glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);

					if (textureReferenceCache.use)
						if (!textureReferenceCache.containsKey(path))
							textureReferenceCache.add(path, tex);
				}
				else {
					Debug::oss << "Image loaing failed: " << path;
					Debug::writeLn();
				}
			}
		}
	}
	if (debugCache) {
		Debug::oss << "No cache hits (" << path << "): " << (((float)(clock() - startLoad) / (float)CLOCKS_PER_SEC) * 1000.f) << "ms";
		Debug::writeLn();
	}
	return tex;
}
void Mesh::setTexture(std::string texturePath, std::string specularPath, std::string normalPath)
{
	material.diffuseTex = Mesh::loadTexture(texturePath, GL_TEXTURE0, useTextureReferenceCache);
	material.specularTex = Mesh::loadTexture(specularPath, GL_TEXTURE1, useTextureReferenceCache);
	material.normalTex = Mesh::loadTexture(normalPath, GL_TEXTURE2, useTextureReferenceCache);
}
void Mesh::make(std::string obj, std::string texturePath, std::string specularPath, std::string normalPath)
{
	loadFromOBJ(obj);
	setTexture(texturePath, specularPath, normalPath);
	load();
	if (geometryReferenceCache.use && useGeometryReferenceCache)
		if (!geometryReferenceCache.containsKey(obj)) {
			geometryReferenceCache.add(obj, new unsigned int[] { mVBO, mVAO, numTris });
		}
}
Vec3 Mesh::calcTangent(Triangle tri)
{
	Vec3 edge1 = tri.v[1].p - tri.v[0].p;
	Vec3 edge2 = tri.v[2].p - tri.v[0].p;
	Vec2 dUV1 = tri.v[1].t - tri.v[0].t;
	Vec2 dUV2 = tri.v[2].t - tri.v[0].t;

	float f = utils3d::fastReciprocal(dUV1.x * dUV2.y - dUV2.x * dUV1.y);

	Vec3 tan;
	tan.x = f * (dUV2.y * edge1.x - dUV1.y * edge2.x);
	tan.y = f * (dUV2.y * edge1.y - dUV1.y * edge2.y);
	tan.z = f * (dUV2.y * edge1.z - dUV1.y * edge2.z);

	return tan;
}
void Mesh::render(Shader* shader, Camera c, Mat4* projectionMatrix)
{
	if (loaded) {
		shader->use();
		shader->setUniformInt("material.diffuse", 0);
		shader->setUniformInt("material.specular", 1);
		shader->setUniformInt("material.normal", 2);
		glBindVertexArray(mVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.diffuseTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material.specularTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material.normalTex);
		shader->setUniformFloat("u_time", glfwGetTime());
		shader->setUniformMat4("model", makeWorldMatrix());
		shader->setUniformVec3("viewPos", c.position);
		shader->setUniformMat4("view", c.makeCameraMatrix().qInverse());
		shader->setUniformMat4("projection", *projectionMatrix);
		shader->setUniformMat4("invModel", makeWorldMatrix().qInverse());
		shader->setUniformFloat("material.shininess", material.shininess);
		glDrawArrays(GL_TRIANGLES, 0, numTris * 3);
	}
}
void Mesh::renderAll(Shader* shader, Camera c, Mat4* projectionMatrix, std::vector<Mesh> m) {
	int sz = m.size();
	shader->setUniformInt("material.diffuse", 0);
	shader->setUniformInt("material.specular", 1);
	shader->setUniformInt("material.normal", 2);
	shader->setUniformFloat("u_time", glfwGetTime());
	shader->setUniformMat4("projection", *projectionMatrix);
	shader->setUniformMat4("view", c.makeCameraMatrix().qInverse());
	shader->setUniformVec3("viewPos", c.position);

	for (int i = 0; i < sz; i++) {
		if (m[i].loaded) {
			glBindVertexArray(m[i].mVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m[i].material.diffuseTex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m[i].material.specularTex);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m[i].material.normalTex);

			shader->setUniformMat4("model", m[i].makeWorldMatrix());
			shader->setUniformMat4("invModel", m[i].makeWorldMatrix().qInverse());
			shader->setUniformFloat("material.shininess", m[i].material.shininess);
			glDrawArrays(GL_TRIANGLES, 0, m[i].numTris * 3);
		}
	}
};
Vec3 Camera::lookVector()
{
	Vec3 target = { 0, 0, 1 };
	Vec3 up = { 0, 1, 0 };
	Mat4 mRotation = Mat4::makeRotationX(rotation.x) * Mat4::makeRotationY(rotation.y) * Mat4::makeRotationZ(rotation.z);
	target = (target * mRotation);
	return target;
}
Mat4 Camera::makeCameraMatrix()
{
	Vec3 vUp = { 0, 1, 0 };
	Vec3 vTarget = { 0, 0, 1 };
	Mat4 matCameraRotY = Mat4::makeRotationY(rotation.y);
	Mat4 matCameraRotX = Mat4::makeRotationX(rotation.x);
	Mat4 matCameraRotZ = Mat4::makeRotationZ(rotation.z);
	Vec3 camRot = vTarget * (matCameraRotX * matCameraRotY * matCameraRotZ);
	vTarget = position + camRot;
	Mat4 matCamera = Mat4::makePointedAt(position, vTarget, vUp);
	return matCamera;
}
VertexArray::~VertexArray()
{
	if (mVBO != GL_NONE)
	{
		glDeleteBuffers(1, &mVBO);
	}
	if (mVAO != GL_NONE)
	{
		glDeleteVertexArrays(1, &mVAO);
	}
}
VertexArray::VertexArray(float* vertPositions, int vertPositionsCount, unsigned int step, unsigned int drawMode)
{
	mVertexCount = vertPositionsCount / 3;

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glBufferData(GL_ARRAY_BUFFER, vertPositionsCount * sizeof(float), vertPositions, drawMode);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, step, NULL);
}
void VertexArray::draw()
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
}
Shader::Shader(std::string vertexShader, std::string fragmentShader, std::string geometryShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(getFileAsString(vertexShader), GL_VERTEX_SHADER);
	unsigned int fs = compileShader(getFileAsString(fragmentShader), GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	if (geometryShader != "NONE") {
		unsigned int gs = compileShader(getFileAsString(geometryShader), GL_GEOMETRY_SHADER);
		glAttachShader(program, gs);
	}
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	mShader = program;
}
std::string Shader::getFileAsString(std::string path)
{
	//std::ifstream f;
	//std::stringstream ss;
	//std::string ln;
	//f.open(path);
	//while (!f.eof()) {
	//	getline(f, ln);
	//	ss << ln << "\n";
	//}
	//return ss.str();
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}
void Shader::use()
{
	glUseProgram(mShader);
}
void Shader::setUniformInt(const char* name, int value)
{
	glUniform1i(getShaderLoc(name), value);
}
void Shader::setUniformBool(const char* name, bool value)
{
	setUniformInt(name, value);
}
void Shader::setUniformFloat(const char* name, float value)
{
	glUniform1f(getShaderLoc(name), value);
}
void Shader::setUniform2F(const char* name, float v1, float v2)
{
	glUniform2f(getShaderLoc(name), v1, v2);
}
void Shader::setUniform3F(const char* name, float v1, float v2, float v3)
{
	glUniform3f(getShaderLoc(name), v1, v2, v3);
}
void Shader::setUniform4F(const char* name, float v1, float v2, float v3, float v4)
{
	glUniform4f(getShaderLoc(name), v1, v2, v3, v4);
}
void Shader::setUniformMat4(const char* name, Mat4 mat)
{
	glUniformMatrix4fv(getShaderLoc(name), 1, false, &mat.m[0][0]);
}
int Shader::getShaderLoc(const char* name)
{
	return glGetUniformLocation(mShader, name);
}
int Shader::compileShader(const std::string& src, unsigned int glType)
{
	unsigned int id = glCreateShader(glType);
	const char* srcCStr = src.c_str();
	glShaderSource(id, 1, &srcCStr, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int eLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &eLength);
		char* message = (char*)alloca(eLength * sizeof(char));
		glGetShaderInfoLog(id, eLength, &eLength, message);
		Debug::oss << "Failed to compile " << " shader: " << message;
		Debug::writeLn();
		glDeleteShader(id);
	}

	return id;
}
void Shader::setUniformVec4(const char* name, Vec3 v4d)
{
	setUniform4F(name, v4d.x, v4d.y, v4d.z, v4d.w);
};
void Shader::setUniformVec3(const char* name, Vec3 v3d)
{
	setUniform3F(name, v3d.x, v3d.y, v3d.z);
};
void Shader::setUniformVec2(const char* name, Vec2 v2d)
{
	setUniform2F(name, v2d.x, v2d.y);
};