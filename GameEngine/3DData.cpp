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
Vector3D operator*(const Matrix4x4& mat, const Vector3D& vec)
{
	return vec * mat;
}
Vector3D operator*(const Vector3D& m1, const float k)
{
	return { m1.x * k, m1.y * k, m1.z * k };
}
Vector3D operator/(const Vector3D& m1, const float k)
{
	return { m1.x / k, m1.y / k, m1.z / k };
}
Vector3D operator*(const Vector3D& v1, const Vector3D& v2)
{
	return { v1.x * v2.x, v1.y + v2.y, v1.z * v2.z, v1.w * v2.w };
}
Vector3D operator/(const Vector3D& v1, const Vector3D& v2)
{
	return { v1.x / v2.x, v1.y + v2.y, v1.z / v2.z, v1.w / v2.w };
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

Vector3D Vector3D::planeIntersect(Vector3D& plane_p, Vector3D& plane_n, Vector3D& lineStart, Vector3D& lineEnd, float& t)
{
	plane_n = plane_n.normalized();
	float plane_d = -Vector3D::dotProduct(plane_n, plane_p);
	float ad = Vector3D::dotProduct(lineStart, plane_n);
	float bd = Vector3D::dotProduct(lineEnd, plane_n);
	t = (-plane_d - ad) / (bd - ad);
	Vector3D lineStartToEnd = lineEnd - lineStart;
	Vector3D lineToIntersect = lineStartToEnd * t;
	return lineStart + lineToIntersect;
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
float* Vector3D::toFloatArray()
{
	float* arr = new float[4];
	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
	arr[3] = w;
	return arr;
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
Matrix4x4 Matrix4x4::makeRotationX(float xAngleRadians)
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
Matrix4x4 Matrix4x4::makeRotationY(float yAngleRadians)
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
Matrix4x4 Matrix4x4::makeRotationZ(float zAngleRadians)
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
Matrix4x4 Matrix4x4::makeTranslation(float x, float y, float z)
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
Matrix4x4 Matrix4x4::makeProjectionMatrix(float fovDegrees, float aspectRatio, float near_, float far_)
{
	float fovRad = 1.0f / tanf(Rotation::toRadians(fovDegrees * 0.5f));
	Matrix4x4 matrix;
	matrix.m[0][0] = aspectRatio * fovRad;
	matrix.m[1][1] = fovRad;
	matrix.m[2][2] = far_ / (far_ - near_);
	matrix.m[3][2] = (-far_ * near_) / (far_ - near_);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}
Matrix4x4 Matrix4x4::makePointedAt(Vector3D& pos, Vector3D& target, Vector3D& up)
{
	Vector3D newForward = target - pos;
	newForward = newForward.normalized();

	Vector3D a = newForward * Vector3D::dotProduct(up, newForward);
	Vector3D newUp = up - a;
	newUp = newUp.normalized();

	Vector3D newRight = Vector3D::crossProduct(newUp, newForward);

	Matrix4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;
}
Matrix4x4 Matrix4x4::makeScale(float x, float y, float z)
{
	Matrix4x4 matrix = makeIdentity();
	matrix.m[0][0] = x;
	matrix.m[1][1] = y;
	matrix.m[2][2] = z;
	return matrix;
}
Matrix4x4 Matrix4x4::qInverse()
{

	Matrix4x4 matrix;
	matrix.m[0][0] = m[0][0]; matrix.m[0][1] = m[1][0]; matrix.m[0][2] = m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m[0][1]; matrix.m[1][1] = m[1][1]; matrix.m[1][2] = m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m[0][2]; matrix.m[2][1] = m[1][2]; matrix.m[2][2] = m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m[3][0] * matrix.m[0][0] + m[3][1] * matrix.m[1][0] + m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m[3][0] * matrix.m[0][1] + m[3][1] * matrix.m[1][1] + m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m[3][0] * matrix.m[0][2] + m[3][1] * matrix.m[1][2] + m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
void Matrix4x4::flatten(float* arr)
{
	arr = new float[16];
	int k = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			arr[k] = m[i][j];
			k++;
		}
}

float Triangle::clipAgainstPlane(Vector3D plane_p, Vector3D plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2)
{
	plane_n.normalize();

	auto dist = [&](Vector3D& p)
	{
		Vector3D n = p.normalized();
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector3D::dotProduct(plane_n, plane_p));
	};

	Vector3D* inside_points[3];  int nInsidePointCount = 0;
	Vector3D* outside_points[3]; int nOutsidePointCount = 0;
	Vector2D* inside_tex[3]; int nInsideTexCount = 0;
	Vector2D* outside_tex[3]; int nOutsideTexCount = 0;

	float d0 = dist(in_tri.p1);
	float d1 = dist(in_tri.p2);
	float d2 = dist(in_tri.p3);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p1; inside_tex[nInsideTexCount++] = &in_tri.t1; }
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p1; outside_tex[nOutsideTexCount++] = &in_tri.t1;
	}
	if (d1 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri.p2; inside_tex[nInsideTexCount++] = &in_tri.t2;
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p2;  outside_tex[nOutsideTexCount++] = &in_tri.t2;
	}
	if (d2 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri.p3; inside_tex[nInsideTexCount++] = &in_tri.t3;
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p3;  outside_tex[nOutsideTexCount++] = &in_tri.t3;
	}

	if (nInsidePointCount == 0)
		return 0;

	if (nInsidePointCount == 3)
	{
		out_tri1 = in_tri;
		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{

		out_tri1.p1 = *inside_points[0];
		out_tri1.t1 = *inside_tex[0];

		float t;
		out_tri1.p2 = Vector3D::planeIntersect(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
		out_tri1.t2.u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.t2.v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.t2.w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		out_tri1.p3 = Vector3D::planeIntersect(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
		out_tri1.t3.u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.t3.v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.t3.w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{

		out_tri1.p1 = *inside_points[0];
		out_tri1.p2 = *inside_points[1];
		out_tri1.t1 = *inside_tex[0];
		out_tri1.t2 = *inside_tex[1];

		float t;
		out_tri1.p3 = Vector3D::planeIntersect(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
		out_tri1.t3.u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.t3.v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.t3.w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		out_tri2.p1 = *inside_points[1];
		out_tri2.t1 = *inside_tex[1];
		out_tri2.p2 = out_tri1.p3;
		out_tri2.t2 = out_tri1.t3;
		out_tri2.p3 = Vector3D::planeIntersect(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
		out_tri2.t3.u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
		out_tri2.t3.v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
		out_tri2.t3.w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
		return 2; // Return two newly formed triangles which form a quad
	}
}

bool Mesh::loadFromOBJ(std::string fileName, bool hasTexture)
{
	using namespace std;
	ifstream f(fileName);
	if (!f.is_open())
		return false;

	vector<Vector3D> verts;
	vector<Vector2D> texs;

	while (!f.eof()) {
		char line[MAX_OBJ_LINE_CHARS];
		char junk;
		f.getline(line, MAX_OBJ_LINE_CHARS);
		stringstream s;

		s << line;
		if (line[0] == 'v') {
			if (line[1] == 't') {
				Vector2D v;
				s >> junk >> junk >> v.u >> v.v;
				texs.push_back(v);
			}
			else {
				Vector3D v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}
		if (!hasTexture) {
			if (line[0] == 'f') {
				int face[3] = { 0 };
				s >> junk >> face[0] >> face[1] >> face[2];
				int v1 = face[0] - 1;
				int v2 = face[1] - 1;
				int v3 = face[2] - 1;
				Triangle fTri;
				fTri.p1 = verts[v1 < 0 ? 0 : v1];
				fTri.p2 = verts[v2 < 0 ? 0 : v2];
				fTri.p3 = verts[v3 < 0 ? 0 : v3];
				tris.push_back(fTri);
			}
		}
		else {
			if (line[0] == 'f') {
				s >> junk;

				string tokens[6];
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
				push.p1 = verts[stoi(tokens[0]) - 1];
				push.p2 = verts[stoi(tokens[2]) - 1];
				push.p3 = verts[stoi(tokens[4]) - 1];
				push.t1 = texs[stoi(tokens[1]) - 1];
				push.t2 = texs[stoi(tokens[3]) - 1];
				push.t3 = texs[stoi(tokens[5]) - 1];

				tris.push_back(push);
			}
		}
	}
	return true;
}
Matrix4x4 Mesh::makeWorldMatrix()
{
	Matrix4x4 matRotY = Matrix4x4::makeRotationY(rotation.y);
	Matrix4x4 matRotX = Matrix4x4::makeRotationX(rotation.x);
	Matrix4x4 matRotZ = Matrix4x4::makeRotationZ(rotation.z);
	Matrix4x4 matTrans = Matrix4x4::makeTranslation(position.x, position.y, position.z);
	Matrix4x4 matScale = Matrix4x4::makeScale(scale.x, scale.y, scale.z);
	Matrix4x4 matWorld = matRotX * matRotY * matRotZ * matTrans * matScale;
	return matWorld;
}
void Mesh::toVertexArray(VertexArray** ptr)
{
	std::vector<float> data;

	for (int i = 0; i < tris.size(); i++) {
		float* arr = tris[i].p1.toFloatArray();
		for (int k = 0; k < 3; k++)
			data.push_back(arr[k]);
		delete arr;
		arr = tris[i].p2.toFloatArray();
		for (int k = 0; k < 3; k++)
			data.push_back(arr[k]);
		delete arr;
		arr = tris[i].p3.toFloatArray();
		for (int k = 0; k < 3; k++)
			data.push_back(arr[k]);
		delete arr;
	}

	*ptr = new VertexArray(data.data(), tris.size() * 9, 0, GL_STATIC_DRAW);
}
void Mesh::load() {
	if (!loaded) {
#if RENDERER == RENDERER_OPENGL
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		void* start = &tris[0];
		void* end = &tris[tris.size() - 1];
		void* p1 = &tris[0].p1;
		void* t1 = &tris[0].t1;
		void* p2 = &tris[0].p2;
		void* t2 = &tris[0].t2;
		void* p3 = &tris[0].p3;
		void* t3 = &tris[0].t3;
		Debug::oss << "Start: " << start << "\n" << "End: " << end << "\np1: " << p1 << "\nt1: " << t1 << "\np2: " << p2 << "\nt2: " << t2 << "\np3: " << p3 << "\nt3: " << t3;
		Debug::writeLn();

		glBufferData(GL_ARRAY_BUFFER, tris.size() * sizeof(Triangle), start, GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vector2D), NULL);
#endif
	}
	loaded = true;
}

Vector3D Camera::lookVector()
{
	Vector3D target = { 0, 0, 1 };
	Vector3D up = { 0, 1, 0 };
	Matrix4x4 mRotation = Matrix4x4::makeRotationX(rotation.x) * Matrix4x4::makeRotationY(rotation.y) * Matrix4x4::makeRotationZ(rotation.z);
	target = (target * mRotation);
	return target;
}
Matrix4x4 Camera::makeCameraMatrix()
{
	Vector3D vUp = { 0, 1, 0 };
	Vector3D vTarget = { 0, 0, 1 };
	Matrix4x4 matCameraRotY = Matrix4x4::makeRotationY(rotation.y);
	Matrix4x4 matCameraRotX = Matrix4x4::makeRotationX(rotation.x);
	Matrix4x4 matCameraRotZ = Matrix4x4::makeRotationZ(rotation.z);
	Vector3D camRot = vTarget * (matCameraRotX * matCameraRotY * matCameraRotZ);
	vTarget = position + camRot;
	Matrix4x4 matCamera = Matrix4x4::makePointedAt(position, vTarget, vUp);
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
