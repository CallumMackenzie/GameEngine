#include "Ingenium3D.h"

using namespace ingenium3D;

Ingenium3D* Ingenium3D::engine3D;
void ingenium3D::Ingenium3D::init()
{
	onCreate();
	refreshProjectionMatrix();
}
Mat4 Ingenium3D::makeTransProjMatrix(Mesh mesh)
{
	Mat4 ret;
	Mat4 matWorld = mesh.makeWorldMatrix();
	Mat4 matCamera = camera.makeCameraMatrix();
	Mat4 matView = matCamera.qInverse();
	ret = (matWorld * matView) * projectionMatrix;
	return ret;
}
void Ingenium3D::refreshProjectionMatrix()
{
	projectionMatrix = Mat4::makeProjectionMatrix(camera.FOV, drwn->aspectRatio[1] / drwn->aspectRatio[0], camera.clipNear, camera.clipFar);
}
void Ingenium3D::setFOV(float fov)
{
	camera.FOV = fov;
	refreshProjectionMatrix();
}
Ingenium3D::Ingenium3D() : Ingenium2D()
{
}
Ingenium3D* Ingenium3D::getEngine()
{
	if (Ingenium3D::engine3D == nullptr) {
		Ingenium3D::engine3D = new Ingenium3D();
	}
	return Ingenium3D::engine3D;
}