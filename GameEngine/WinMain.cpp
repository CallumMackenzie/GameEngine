#include "Memory.h"

#ifdef _DEBUG
MEM_COUNTER
#endif

#include "Ingenium.h"

// using namespace ingenium2D;
using namespace ingenium3D;

// Callum Mackenzie

struct Ingenium : Ingenium3D
{
	Mesh cube;
	Mesh testMesh;

	void onCreate()
	{
		Ingenium2D::engine = this;
		Ingenium3D::engine3D = this;
		primeClass->setWindowProc(DEFAULT_3D_WND_PROC);
		primeClass->wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		primeClass->registerClass();

		RootWindow* win = new RootWindow(primeClass->hInst, primeClass, L"Ingenium", CW_USEDEFAULT, CW_USEDEFAULT, 950, 1600);
		win->style = WS_SYSMENU | WS_SIZEBOX;
		win->create();

		// win->setFullscreen();
		win->show();

		drwn = new ingenium2D::Direct2DWindow(win);
		drwn->clearColour = D2D1::ColorF::White;
		drwn->drawQueue(false);

		updateDepthBuffer();

		Time::setFixedFPS(30);

		cube.scale = 1;
		cube.position = { 0, 0, 10 };
		camera.position = { 0, 0, -5 };
		camera.FOV = 75;
		if (!cube.loadFromOBJ("./cube.obj")) {
			abort();
		};

		testMesh.loadFromOBJ("D:\\MAND.obj");
		testMesh.position = { 0, 0, 0 };
		testMesh.scale = 0.1;
	};
	void onUpdate() {
		// Vector3D vec = { -0.001 * Time::deltaTime, 0.001 * Time::deltaTime, 0.001 * Time::deltaTime, 0 };
		// cube.rotation = cube.rotation + vec;
		float speed = 0;
		float cameraMoveSpeed = Rotation::toRadians(0.1);
		Vector3D foreward = camera.lookVector();
		Vector3D height;
		Vector3D move;
		Vector3D rotate;
		Input* in = Input::getInput();
		if (in->getKeyState(87))
			speed = 0.01;
		if (in->getKeyState(83))
			speed = -0.01;
		if (in->getKeyState(68))
			move.x = 0.01;
		if (in->getKeyState(65))
			move.x = -0.01;

		if (in->getKeyState(81))
			height.y = 0.01;
		if (in->getKeyState(69))
			height.y = -0.01;

		if (in->getKeyState(37))
			rotate.y = -cameraMoveSpeed;
		if (in->getKeyState(39))
			rotate.y = cameraMoveSpeed;

		if (in->getKeyState(38))
			rotate.x = -cameraMoveSpeed;
		if (in->getKeyState(40))
			rotate.x = cameraMoveSpeed;

		//if (in->getKeyState(88))
		//	rotate.z = -0.005;
		//if (in->getKeyState(67))
		//	rotate.z = 0.005;

		camera.rotation = camera.rotation + (rotate * Time::deltaTime);
		camera.position = camera.position + ((foreward * speed) + move + height) * Time::deltaTime;
		drwn->beginRender();
		drwn->pRT->Clear(drwn->clearColour);
		renderMesh(cube);
		renderMesh(testMesh);
		drwn->endRender();
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Ingenium* eg = new Ingenium();
	eg->start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	delete eg;

#ifdef _DEBUG
	printAllocationInfo();
#endif

	return 0;
}