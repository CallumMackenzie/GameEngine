#include <stdlib.h>

#include "ModWin.h"
void* operator new (size_t size)
{
    return malloc(size);
}
#include "Log.h"
#include "WindowClass.h"
#include "Window.h"
#include "Direct2D.h"
#include "Direct2DWindow.h"

#include "Engine.h"
#include "Renderable.h"

// Callum Mackenzie

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    Vector2* v2 = new Vector2(10.f, 4.f);
    Debug::oss << "x: " << v2->x() << "  y: " << v2->y();
    Debug::writeLn();

    Debug::oss << "Magnitude: " << v2->magnitude();
    Debug::writeLn();

    v2->normalize();
    Debug::oss << "Normalized x: " << v2->x() << "\nNormalized y: " << v2->y() << "\nNormalized magnitude: " << v2->magnitude();
    Debug::writeLn();

    Vector2* v2_v2_1_operated = Vector2::add(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug::oss << "2 vectors added: x: " << v2_v2_1_operated->x() << "  y: " << v2_v2_1_operated->y();
    Debug::writeLn();

    v2_v2_1_operated = Vector2::subtract(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug::oss << "2 vectors subtracted: x: " << v2_v2_1_operated->x() << "  y: " << v2_v2_1_operated->y();
    Debug::writeLn();

    v2_v2_1_operated = Vector2::multiply(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug::oss << "2 vectors multiplied: x: " << v2_v2_1_operated->x() << "  y: " << v2_v2_1_operated->y();
    Debug::writeLn();

    v2_v2_1_operated = Vector2::divide(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug::oss << "2 vectors divided: x: " << v2_v2_1_operated->x() << "  y: " << v2_v2_1_operated->y();
    Debug::writeLn();

    Vector* vct = new Vector(new float[6]{ 0.f, 1.f, 2.f, 3.f, 4.f, 5.f }, 6);
    Debug::oss << "Custom vector of length " << vct->length << ":\n";
    for (int i = 0; i < vct->length; i++) 
    {
        Debug::oss <<  i << ": " << vct->vector[i] << "\n";
    }
    Debug::writeLn();

    // Renderable<ID2D1Bitmap*>* rd = new Renderable<ID2D1Bitmap*>("ID");
    

    //Engine* e = Engine::getEngine();
    //e->init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    //MSG msg;
    //while (Engine::getEngine()->running) 
    //{
    //    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}

    //delete Engine::getEngine();

    return 0;
}