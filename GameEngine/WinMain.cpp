#include <stdlib.h>

#include "ModWin.h"
void* operator new (size_t size)
{
	return malloc(size);
}
#include "Engine.h"

// Callum Mackenzie

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Engine* e = Engine::getEngine();
	e->init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	if (Engine::debug) {
		Debug::createDebugWindow(hInstance);
	}

	MSG msg;
	while (Engine::getEngine()->running)
	{
		
		if (Time::getTime()->nextFrameReady() && Engine::getEngine()->performanceMode) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Engine::getEngine()->onUpdate();
		}

		if (!Engine::getEngine()->performanceMode) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Sleep(Time::getTime()->targetDeltaTime * 1000.0);
		}

		if (Engine::debug) {
			if (Debug::windowWriteReady()) {
				Debug::oss << "FPS: " << (1000.f / Time::getTime()->deltaTime) << "\n";
				Debug::writeToWindow();
			}
		}
	}

	if (Engine::debug) {
		Debug::destroyDebugWindow();
	}
	delete Engine::getEngine();

	return 0;
}