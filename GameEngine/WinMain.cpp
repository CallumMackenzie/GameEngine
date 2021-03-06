#include <stdlib.h>

#ifdef _DEBUG
int memManagementTracker = 0;

void* operator new (size_t size)
{
	memManagementTracker++;
	return malloc(size);
}
void operator delete(void* p)
{
	memManagementTracker--;
	free(p);
}
#endif

#include "ModWin.h"
#include "Engine.h"

// #define INGENIUM_WND_GUI 1
// Callum Mackenzie

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
#if defined(SCRIPT_LUA)
	ingenium_lua::initLua();
#endif

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

		if (Engine::getEngine()->running) {
			if (Time::getTime()->nextFrameReady()) {
				Engine::getEngine()->onUpdate();
#if defined(_DEBUG) && defined(INGENIUM_WND_GUI)
				if (Debug::windowWriteReady()) {
					Debug::oss << "FPS: " << (1000.f / Time::getTime()->deltaTime) << "\n";
					Debug::writeToWindow();
				}
				Debug::oss.str("");
				Debug::oss.clear();
#endif
			}
		}
	}

#if defined(_DEBUG) && defined(INGENIUM_WND_GUI)
	Debug::destroyDebugWindow();
#endif
	delete Engine::getEngine();

#ifdef _DEBUG
	Debug::oss << (memManagementTracker >= 0 ? "Objects not deallocated: " : "Extra objects deleted: ") << memManagementTracker;
#endif
	Debug::writeLn();

	return 0;
}