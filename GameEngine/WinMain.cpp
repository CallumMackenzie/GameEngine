#include <stdlib.h>

#include "ModWin.h"
void* operator new (size_t size)
{
	return malloc(size);
}
#include "Engine.h"

// #define INGENIUM_WND_GUI
// Callum Mackenzie

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Engine* e = Engine::getEngine();
	e->init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

#if defined(_DEBUG) && defined(INGENIUM_WND_GUI)
	Debug::createDebugWindow(hInstance);
#endif

	MSG msg;
	while (Engine::getEngine()->running)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (Time::getTime()->nextFrameReady()) {
			Engine::getEngine()->onUpdate();
		}

#if defined(_DEBUG) && defined(INGENIUM_WND_GUI)
		if (Debug::windowWriteReady()) {
			Debug::oss << "FPS: " << (1000.f / Time::getTime()->deltaTime) << "\n";
			Debug::writeToWindow();
		}
#endif
	}

#if defined(_DEBUG) && defined(INGENIUM_WND_GUI)
	Debug::destroyDebugWindow();
#endif
	delete Engine::getEngine();

	return 0;
}