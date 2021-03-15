#include "Ingenium3D.h"

using namespace ingenium3D;

Ingenium3D* Ingenium3D::engine3D;

void Ingenium3D::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	primeClass = new WindowClass(L"Ingenium WC", hInstance);
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
void Ingenium3D::renderMesh(Mesh mesh)
{
	Matrix4x4 matRotY = Matrix4x4::makeRotationY(mesh.rotation.y);
	Matrix4x4 matRotX = Matrix4x4::makeRotationX(mesh.rotation.x);
	Matrix4x4 matRotZ = Matrix4x4::makeRotationZ(mesh.rotation.z);

	Matrix4x4 matTrans = Matrix4x4::makeTranslation(0.0f, 0.0f, 5.0f);

	Matrix4x4 matWorld = Matrix4x4::makeIdentity();	// Form World Matrix
	matWorld = matRotZ * matRotX * matRotY; // Transform by rotation
	matWorld = matWorld * matTrans; // Transform by translation

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
		triTransformed.p[0] = matWorld * (tri.p[0] * mesh.scale);
		triTransformed.p[1] = matWorld * (tri.p[1] * mesh.scale);
		triTransformed.p[2] = matWorld * (tri.p[2] * mesh.scale);
		triTransformed.t[0] = tri.t[0];
		triTransformed.t[1] = tri.t[1];
		triTransformed.t[2] = tri.t[2];

		// Calculate triangle Normal
		Vector3D normal, line1, line2;

		// Get lines either side of triangle
		line1 = triTransformed.p[1] - triTransformed.p[0];
		line2 = triTransformed.p[2] - triTransformed.p[0];

		// Take cross product of lines to get normal to triangle surface
		normal = Vector3D::crossProduct(line1, line2);

		// You normally need to normalise a normal!
		normal = normal.normalized();

		// Get Ray from triangle to camera
		Vector3D vCameraRay = triTransformed.p[0] - camera.position;

		// If ray is aligned with normal, then triangle is visible
		if (Vector3D::dotProduct(normal, vCameraRay) < 0.0f)
		{
			// Illumination
			Vector3D light_direction = { 0.0f, 1.0f, -1.0f };
			light_direction = light_direction.normalized();

			// How "aligned" are light direction and triangle surface normal?
			float dp = std::max(0.1f, Vector3D::dotProduct(light_direction, normal));

			// Choose console colours as required (much easier with RGB)
			triTransformed.col = tri.col;

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
			nClippedTriangles = Triangle::clipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

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

	for (int i = 0; i < drwn->screenWidth() * drwn->screenHeight(); i++)
		depthBuffer[i] = 0.0f;

	for (auto& triToRaster : vecTrianglesToRaster)
	{
		// Clip triangles against all four screen edges, this could yield
		// a bunch of triangles, so create a queue that we traverse to 
		//  ensure we only test new triangles generated against planes
		Triangle clipped[2];
		std::list<Triangle> listTriangles;

		// Add initial triangle
		listTriangles.push_back(triToRaster);
		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				// Take triangle from front of queue
				Triangle test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane. I like how this
				// comment is almost completely and utterly justified
				switch (p)
				{
				case 0:	nTrisToAdd = Triangle::clipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = Triangle::clipAgainstPlane({ 0.0f, (float)drwn->screenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = Triangle::clipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = Triangle::clipAgainstPlane({ (float)drwn->screenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				// Clipping may yield a variable number of triangles, so
				// add these new ones to the back of the queue for subsequent
				// clipping against next planes
				for (int w = 0; w < nTrisToAdd; w++)
					listTriangles.push_back(clipped[w]);
			}
			nNewTriangles = listTriangles.size();
		}

		// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
		for (auto& t : listTriangles)
		{
			// TexturedTriangle(t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w, t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w, t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w, sprTex1);

			// FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, t.sym, t.col);
			// DrawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, PIXEL_SOLID, FG_WHITE);
			drwn->drawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, drwn->pBlackBrush);
		}
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