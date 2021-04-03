#pragma once
#include <list>
#include <algorithm>
#include "Ingenium2D.h"
#include "3DData.h"
#include "Lighting.h"

namespace ingenium3D
{
	class Ingenium3D : public ingenium2D::Ingenium2D
	{
	public:
		Ingenium3D();

		static Ingenium3D* engine3D;

		static Ingenium3D* getEngine();
		virtual void init();

		Mat4 makeTransProjMatrix(Mesh m);

		void refreshProjectionMatrix();
		void setFOV(float fovDegrees);

	public:
		static ID2D1PathGeometry* pGTriangle;

		float *depthBuffer = nullptr;
		Mat4 projectionMatrix;
		Camera camera;
	};
}