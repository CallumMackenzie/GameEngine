#include "Ingenium3D.h"

using namespace ingenium3D;

Ingenium3D* Ingenium3D::engine3D;

void Ingenium3D::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
#if RENDERER == RENDERER_DIRECT2D
	primeClass = new WindowClass(L"Ingenium WC", hInstance);
#endif
	onCreate();
	refreshProjectionMatrix();
	updateDepthBuffer();
}
void Ingenium3D::updateDepthBuffer()
{
	if (drwn) {
		memory::safe_delete(Ingenium3D::getEngine()->depthBuffer);
		Ingenium3D::getEngine()->depthBuffer = new float[(float)drwn->screenWidth() * (float)drwn->screenHeight()]{ 0 };
	}
}
void ingenium3D::Ingenium3D::updateBufferMatrix(Mesh mesh)
{
	Matrix4x4 matRotY = Matrix4x4::makeRotationY(mesh.rotation.y);
	Matrix4x4 matRotX = Matrix4x4::makeRotationX(mesh.rotation.x);
	Matrix4x4 matRotZ = Matrix4x4::makeRotationZ(mesh.rotation.z);

	Matrix4x4 matTrans = Matrix4x4::makeTranslation(mesh.position.x, mesh.position.y, mesh.position.z);
	Matrix4x4 matScale = Matrix4x4::makeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

	Matrix4x4 matWorld = Matrix4x4::makeIdentity();	// Form World Matrix
	matWorld = matRotX * matRotY * matRotZ * matTrans * matScale; // Transform by rotation and translation

	// Create "Point At" Matrix for camera
	Vector3D vUp = { 0, 1, 0 };
	Vector3D vTarget = { 0, 0, 1 };
	Matrix4x4 matCameraRotY = Matrix4x4::makeRotationY(camera.rotation.y);
	Matrix4x4 matCameraRotX = Matrix4x4::makeRotationX(camera.rotation.x);
	Matrix4x4 matCameraRotZ = Matrix4x4::makeRotationZ(camera.rotation.z);
	Vector3D camRot = vTarget * (matCameraRotX * matCameraRotY * matCameraRotZ);
	vTarget = camera.position + camRot;
	Matrix4x4 matCamera = Matrix4x4::makePointedAt(camera.position, vTarget, vUp);

	// Make view matrix from camera
	Matrix4x4 matView = matCamera.qInverse();
	bufferMatrix = matView;
}
std::vector<Triangle> ingenium3D::Ingenium3D::getRasterizedMesh(Mesh mesh)
{
	Matrix4x4 matRotY = Matrix4x4::makeRotationY(mesh.rotation.y);
	Matrix4x4 matRotX = Matrix4x4::makeRotationX(mesh.rotation.x);
	Matrix4x4 matRotZ = Matrix4x4::makeRotationZ(mesh.rotation.z);

	Matrix4x4 matTrans = Matrix4x4::makeTranslation(mesh.position.x, mesh.position.y, mesh.position.z);
	Matrix4x4 matScale = Matrix4x4::makeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

	Matrix4x4 matWorld = Matrix4x4::makeIdentity();	// Form World Matrix
	matWorld = matRotX * matRotY * matRotZ * matTrans * matScale; // Transform by rotation and translation

	// Create "Point At" Matrix for camera
	Vector3D vUp = { 0, 1, 0 };
	Vector3D vTarget = { 0, 0, 1 };
	Matrix4x4 matCameraRotY = Matrix4x4::makeRotationY(camera.rotation.y);
	Matrix4x4 matCameraRotX = Matrix4x4::makeRotationX(camera.rotation.x);
	Matrix4x4 matCameraRotZ = Matrix4x4::makeRotationZ(camera.rotation.z);
	Vector3D camRot = vTarget * (matCameraRotX * matCameraRotY * matCameraRotZ);
	vTarget = camera.position + camRot;
	Matrix4x4 matCamera = Matrix4x4::makePointedAt(camera.position, vTarget, vUp);

	// Make view matrix from camera
	Matrix4x4 matView = matCamera.qInverse();

	// Store triagles for rastering later
	std::vector<Triangle> vecTrianglesToRaster;

	// Draw Triangles
	for (auto tri : mesh.tris)
	{
		Triangle triProjected, triTransformed, triViewed;

		// World Matrix Transform
		triTransformed.p[0] = matWorld * tri.p[0];
		triTransformed.p[1] = matWorld * tri.p[1];
		triTransformed.p[2] = matWorld * tri.p[2];
		triTransformed.t[0] = tri.t[0];
		triTransformed.t[1] = tri.t[1];
		triTransformed.t[2] = tri.t[2];

		// Calculate triangle Normal
		Vector3D normal, line1, line2;

		// Get lines either side of triangle
		line1 = triTransformed.p[1] - triTransformed.p[0];
		line2 = triTransformed.p[2] - triTransformed.p[0];

		// Take cross product of lines to get normal to triangle surface
		normal = Vector3D::crossProduct(line1, line2).normalized();

		// Get Ray from triangle to camera
		Vector3D vCameraRay = triTransformed.p[0] - camera.position;

		// If ray is aligned with normal, then triangle is visible
		float aligned = Vector3D::dotProduct(normal, vCameraRay);
		if (aligned < 0.0f)
		{
			// Illumination
			Vector3D light_direction = { 0.0f, 1.0f, -1.0f };
			light_direction.normalize();

			// How "aligned" are light direction and triangle surface normal?
			float dp = std::max(camera.clipNear, Vector3D::dotProduct(light_direction, normal));

			// Choose console colours as required (much easier with RGB
			short r = dp * 255;
			short g = dp * 255;
			short b = dp * 255;
			long colour = (b << 16) + (g << 8) + r;
			triTransformed.col = colour;

			// Convert World Space --> View Space
			triViewed.p[0] = matView * triTransformed.p[0];
			triViewed.p[1] = matView * triTransformed.p[1];
			triViewed.p[2] = matView * triTransformed.p[2];
			triViewed.col = triTransformed.col;
			triViewed.t[0] = triTransformed.t[0];
			triViewed.t[1] = triTransformed.t[1];
			triViewed.t[2] = triTransformed.t[2];

			// Clip Viewed Triangle against near plane, this could form two additional
			// additional triangles. 
			int nClippedTriangles = 0;
			Triangle clipped[2];
			nClippedTriangles = Triangle::clipAgainstPlane({ 0.0f, 0.0f, camera.clipNear }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

			// We may end up with multiple triangles form the clip, so project as
			// required
			for (int n = 0; n < nClippedTriangles; n++)
			{
				// Project triangles from 3D --> 2D
				triProjected.p[0] = projectionMatrix * clipped[n].p[0];
				triProjected.p[1] = projectionMatrix * clipped[n].p[1];
				triProjected.p[2] = projectionMatrix * clipped[n].p[2];
				triProjected.col = clipped[n].col;
				triProjected.t[0] = clipped[n].t[0];
				triProjected.t[1] = clipped[n].t[1];
				triProjected.t[2] = clipped[n].t[2];


				triProjected.t[0].u = triProjected.t[0].u / triProjected.p[0].w;
				triProjected.t[1].u = triProjected.t[1].u / triProjected.p[1].w;
				triProjected.t[2].u = triProjected.t[2].u / triProjected.p[2].w;

				triProjected.t[0].v = triProjected.t[0].v / triProjected.p[0].w;
				triProjected.t[1].v = triProjected.t[1].v / triProjected.p[1].w;
				triProjected.t[2].v = triProjected.t[2].v / triProjected.p[2].w;

				triProjected.t[0].w = 1.0f / triProjected.p[0].w;
				triProjected.t[1].w = 1.0f / triProjected.p[1].w;
				triProjected.t[2].w = 1.0f / triProjected.p[2].w;


				// Scale into view, we moved the normalising into cartesian space
				// out of the matrix.vector function from the previous videos, so
				// do this manually
				triProjected.p[0] = triProjected.p[0] / triProjected.p[0].w;
				triProjected.p[1] = triProjected.p[1] / triProjected.p[1].w;
				triProjected.p[2] = triProjected.p[2] / triProjected.p[2].w;

				// X/Y are inverted so put them back
				triProjected.p[0].x *= -1.0f;
				triProjected.p[1].x *= -1.0f;
				triProjected.p[2].x *= -1.0f;
				triProjected.p[0].y *= -1.0f;
				triProjected.p[1].y *= -1.0f;
				triProjected.p[2].y *= -1.0f;

				// Offset verts into visible normalised space
				Vector3D vOffsetView = { 1,1,0 };
				triProjected.p[0] = triProjected.p[0] + vOffsetView;
				triProjected.p[1] = triProjected.p[1] + vOffsetView;
				triProjected.p[2] = triProjected.p[2] + vOffsetView;
				triProjected.p[0].x *= 0.5f * drwn->screenWidth();
				triProjected.p[0].y *= 0.5f * drwn->screenHeight();
				triProjected.p[1].x *= 0.5f * drwn->screenWidth();
				triProjected.p[1].y *= 0.5f * drwn->screenHeight();
				triProjected.p[2].x *= 0.5f * drwn->screenWidth();
				triProjected.p[2].y *= 0.5f * drwn->screenHeight();

				// Store triangle for sorting
				vecTrianglesToRaster.push_back(triProjected);
			}
		}
	}

	std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](Triangle& t1, Triangle& t2)
		{
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;
		});

	//for (int i = 0; i < drwn->screenWidth() * drwn->screenHeight(); i++)
	//	depthBuffer[i] = 0.0f;
	std::vector<Triangle> finTriangles;

	for (auto& triToRaster : vecTrianglesToRaster)
	{

		Triangle clipped[2];
		std::list<Triangle> listTriangles;

		listTriangles.push_back(triToRaster);
		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				Triangle test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;
				switch (p)
				{
				case 0:	nTrisToAdd = Triangle::clipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = Triangle::clipAgainstPlane({ 0.0f, (float)drwn->screenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = Triangle::clipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = Triangle::clipAgainstPlane({ (float)drwn->screenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				for (int w = 0; w < nTrisToAdd; w++)
					listTriangles.push_back(clipped[w]);
			}
			nNewTriangles = listTriangles.size();
		}

		for (auto& t : listTriangles)
		{
			finTriangles.push_back(t);
		}
	}
	return finTriangles;
}
void Ingenium3D::renderMesh(Mesh mesh)
{
	auto trngls = getRasterizedMesh(mesh);
	for (auto t : trngls) {
#if RENDERER == RENDERER_DIRECT2D
		drwn->drawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, drwn->pBlackBrush);
#endif
#if RENDERER == RENDERER_OPENGL
		Vector2D p1 = drwn->worldScreenSpaceToScreenSpace(t.p[0].x, t.p[0].y);
		Vector2D p2 = drwn->worldScreenSpaceToScreenSpace(t.p[1].x, t.p[1].y);
		Vector2D p3 = drwn->worldScreenSpaceToScreenSpace(t.p[2].x, t.p[2].y);
		drwn->drawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
#endif
	}
}
void Ingenium3D::refreshProjectionMatrix()
{
	projectionMatrix = Matrix4x4::makeProjectionMatrix(camera.FOV, drwn->aspectRatio[1] / drwn->aspectRatio[0], camera.clipNear, camera.clipFar);
}
void Ingenium3D::setFOV(float fov)
{
	camera.FOV = fov;
	refreshProjectionMatrix();
}
Ingenium3D* Ingenium3D::getEngine()
{
	if (Ingenium3D::engine3D == nullptr) {
		Ingenium3D::engine3D = new Ingenium3D();
	}
	return Ingenium3D::engine3D;
}
#if RENDERER == RENDERER_DIRECT2D
LRESULT CALLBACK Ingenium3D::DEFAULT_3D_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		Ingenium3D::getEngine()->updateDepthBuffer();
		break;
	default:
		break;
	}
	return DEFAULT_WND_PROC(hwnd, uMsg, wParam, lParam);
};
#endif
/*
void Ingenium3D::texturedTriangle(int x1, int y1, float u1, float v1, float w1, int x2, int y2, float u2, float v2, float w2, int x3, int y3, float u3, float v3, float w3)
{
	if (y2 < y1)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
		std::swap(u1, u2);
		std::swap(v1, v2);
		std::swap(w1, w2);
	}

	if (y3 < y1)
	{
		std::swap(y1, y3);
		std::swap(x1, x3);
		std::swap(u1, u3);
		std::swap(v1, v3);
		std::swap(w1, w3);
	}

	if (y3 < y2)
	{
		std::swap(y2, y3);
		std::swap(x2, x3);
		std::swap(u2, u3);
		std::swap(v2, v3);
		std::swap(w2, w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	float tex_u, tex_v, tex_w;

	float dax_step = 0, dbx_step = 0,
		du1_step = 0, dv1_step = 0,
		du2_step = 0, dv2_step = 0,
		dw1_step = 0, dw2_step = 0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2);
	if (dy2) dw2_step = dw2 / (float)abs(dy2);

	if (dy1)
	{
		for (int i = y1; i <= y2; i++)
		{
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > depthBuffer[i * (int)drwn->screenWidth() + j])
				{
					drwn->drawTriangle(x1, y1, x2, y2, x3, y3, drwn->pBlackBrush);
					depthBuffer[i * (int)drwn->screenWidth() + j] = tex_w;
				}
				t += tstep;
			}

		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;

				if (tex_w > depthBuffer[i * (int)drwn->screenWidth() + j])
				{
					drwn->drawTriangle(x1, y1, x2, y2, x3, y3, drwn->pBlackBrush);
					depthBuffer[i * (int)drwn->screenWidth() + j] = tex_w;
				}
				t += tstep;
			}
		}
	}
}
*/