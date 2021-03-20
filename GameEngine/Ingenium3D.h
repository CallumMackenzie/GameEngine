#pragma once
#include <list>
#include <algorithm>
#include "Ingenium2D.h"
#include "3DData.h"

namespace ingenium3D
{
	class Ingenium3D : public ingenium2D::Ingenium2D
	{
	public:
		static Ingenium3D* engine3D;

		static Ingenium3D* getEngine();
#if RENDERER == RENDERER_DIRECT2D
		static LRESULT CALLBACK DEFAULT_3D_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

		virtual void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow);
		void updateDepthBuffer();
		std::vector<Triangle> getRasterizedMesh(Mesh mesh);
		void renderMesh(Mesh mesh);
		void refreshProjectionMatrix();
		void setFOV(float fovDegrees);

		// void texturedTriangle(int x1, int y1, float u1, float v1, float w1, int x2, int y2, float u2, float v2, float w2, int x3, int y3, float u3, float v3, float w3);

		void updateBufferMatrix(Mesh mesh);

	public:
		static ID2D1PathGeometry* pGTriangle;

		float *depthBuffer = nullptr;
		Matrix4x4 projectionMatrix;
		Matrix4x4 bufferMatrix;
		Camera camera;
	};
}