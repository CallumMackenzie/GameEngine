#include "Memory.h"

MEM_COUNTER

#include "Ingenium.h"

using namespace ingenium2D;

// Callum Mackenzie

struct Ingenium : Ingenium2D  
{
	void onCreate() 
	{
		Ingenium2D::engine = this;
		primeClass->setWindowProc(DEFAULT_WND_PROC);
		primeClass->wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		primeClass->registerClass();

		RootWindow* win = new RootWindow(primeClass->hInst, primeClass, L"Ingenium", CW_USEDEFAULT, CW_USEDEFAULT, 900, 1600);
		win->style = WS_SYSMENU | WS_SIZEBOX;
		win->create();
		win->show();

		drwn = new Direct2DWindow(win);
		drwn->drawQueue(false);

		Time::getTime()->setFixedFPS(30);
	};
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Ingenium* eg = new Ingenium();
	eg->start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	delete eg;

	printAllocationInfo();

	return 0;
}