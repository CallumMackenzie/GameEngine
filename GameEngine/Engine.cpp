#include "Engine.h"

// Callum Mackenzie

Engine* Engine::engine;

Engine* Engine::getEngine()
{
	if (engine == nullptr) {
		engine = new Engine();
	}
	return engine;
}

Engine::Engine() 
{
	running = true;
}

Engine::~Engine() {
	if (drwn != nullptr) {
		delete drwn;
	}
	if (primeClass != nullptr) {
		delete primeClass;
	}
}

void Engine::stop() 
{
	running = false;
	if (gameObjects != nullptr) {
		delete gameObjects;
	}
	if (sprites != nullptr) {
		delete sprites;
	}
	if (Input::getInput() != nullptr) {
		delete Input::getInput();
	}
}

void Engine::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	// HICON hIcon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	primeClass = new WindowClass(L"Prime Window Class", hInstance);
	primeClass->setWindowProc(DEFAULT_WND_PROC);
	primeClass->wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//primeClass->wc.hIcon = hIcon;
	//primeClass->wc.hIconSm = hIcon;
	primeClass->registerClass();

	RootWindow* win = new RootWindow(hInstance, primeClass, L"Window", 0, 0, CW_USEDEFAULT, CW_USEDEFAULT);
	win->style = WS_SYSMENU; 
	win->create();
	win->setFullscreen();
	win->show();

	////HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE());

	drwn = new Direct2DWindow(win);
	drwn->clearColour = D2D1::ColorF::Black;
	drwn->beginRender();
	drwn->drawQueue(false);
	drwn->endRender();
}

LRESULT CALLBACK Engine::DEFAULT_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		Engine::getEngine()->stop();
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		Input::getInput()->MouseClick[0] = true;
		break;
	case WM_LBUTTONUP:
		Input::getInput()->MouseClick[0] = false;
		break;
	case WM_RBUTTONDOWN:
		Input::getInput()->MouseClick[1] = true;
		break;
	case WM_RBUTTONUP:
		Input::getInput()->MouseClick[1] = false;
		break;
	case WM_MBUTTONDOWN:
		Input::getInput()->MouseClick[2] = true;
		break;
	case WM_MBUTTONUP:
		Input::getInput()->MouseClick[2] = false;
		break;
	case WM_MOUSELEAVE || WM_NCMOUSELEAVE || WM_SETFOCUS:
		Input::getInput()->MouseClick[0] = false;
		Input::getInput()->MouseClick[1] = false;
		Input::getInput()->MouseClick[2] = false;
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
};