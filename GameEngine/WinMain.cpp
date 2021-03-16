#include "Memory.h"

#ifdef _DEBUG
MEM_COUNTER
#endif

#include "Ingenium.h"

using namespace ingenium2D;
// using namespace ingenium3D;

// Callum Mackenzie

//struct Ingenium : Ingenium2D
//{
//	void onCreate()
//	{
//		Ingenium2D::engine = this;
//		primeClass->setWindowProc(DEFAULT_WND_PROC);
//		primeClass->wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//		primeClass->registerClass();
//
//		RootWindow* win = new RootWindow(primeClass->hInst, primeClass, L"Ingenium", CW_USEDEFAULT, CW_USEDEFAULT, 950, 1600);
//		win->style = WS_SYSMENU | WS_SIZEBOX;
//		win->create();
//
//		// win->setFullscreen();
//		win->show();
//
//		drwn = new ingenium2D::Direct2DWindow(win);
//		drwn->clearColour = D2D1::ColorF::White;
//		drwn->drawQueue(false);
//
//
//		Time::setFixedFPS(30);
//	};
//	void onUpdate() {
//
//	}
//};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Ingenium2D* eg = new Ingenium2D();
	eg->start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	// delete eg;

#ifdef _DEBUG
	printAllocationInfo();
#endif

	return 0;
}