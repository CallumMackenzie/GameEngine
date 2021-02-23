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

    MSG msg;
    while (Engine::getEngine()->running) 
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    delete Engine::getEngine();

    return 0;
}