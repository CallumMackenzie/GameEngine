#pragma once
#include <list>
#include "Ingenium2D.h"
#include "3DData.h"

namespace ingenium3D
{
	class Ingenium3D : public ingenium2D::Ingenium2D
	{
	public:
		static Ingenium3D* engine3D;

		static Ingenium3D* getEngine();
		static LRESULT CALLBACK DEFAULT_3D_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow);
		void updateDepthBuffer();
		void renderMesh(Mesh mesh);
		void refreshProjectionMatrix();
		void setFOV(float fov);

	public:
		float *depthBuffer = nullptr;
		Matrix4x4 projectionMatrix;
		Camera camera;
	};
}