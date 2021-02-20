#include "ModWin.h"
#include "WindowClass.h"
#include "Window.h"
#include "Direct2D.h"
#include "Direct2DWindow.h"

#include "Log.h"
#include "Engine.h"
#include "Renderable.h"

// Callum Mackenzie

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    Vector2* v2 = new Vector2(10.f, 4.f);
    Debug<const char*>::Log ("x: ");
    Debug<float>::LogLine(v2->x());
    Debug<const char*>::Log("y: ");
    Debug<float>::LogLine(v2->y());

    Debug<const char*>::Log("Magnitude: ");
    Debug<float>::LogLine(v2->magnitude());

    v2->normalize();
    Debug<const char*>::Log("Normalized x: ");
    Debug<float>::LogLine(v2->x());
    Debug<const char*>::Log("Normalized y: ");
    Debug<float>::LogLine(v2->y());
    Debug<const char*>::Log("Normalized magnitude: ");
    Debug<float>::LogLine(v2->magnitude());

    Debug<const char*>::Log("2 vectors added: x: ");
    Vector2* v2_v2_1_operated = Vector2::add(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug<float>::Log(v2_v2_1_operated->x());
    Debug<const char*>::Log("  y: ");
    Debug<float>::LogLine(v2_v2_1_operated->y());

    Debug<const char*>::Log("2 vectors subtracted: x: ");
    v2_v2_1_operated = Vector2::subtract(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug<float>::Log(v2_v2_1_operated->x());
    Debug<const char*>::Log("  y: ");
    Debug<float>::LogLine(v2_v2_1_operated->y());

    Debug<const char*>::Log("2 vectors multiplied: x: ");
    v2_v2_1_operated = Vector2::multiply(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug<float>::Log(v2_v2_1_operated->x());
    Debug<const char*>::Log("  y: ");
    Debug<float>::LogLine(v2_v2_1_operated->y());

    Debug<const char*>::Log("2 vectors divided: x: ");
    v2_v2_1_operated = Vector2::divide(new Vector2(10.f, 4.f), new Vector2(3.f, 2.f));
    Debug<float>::Log(v2_v2_1_operated->x());
    Debug<const char*>::Log("  y: ");
    Debug<float>::LogLine(v2_v2_1_operated->y());

    Debug<const char*>::Log("Custom vector: \nLength:");
	Vector* vct = new Vector(new float[6]{ 0.f, 1.f, 2.f, 3.f, 4.f, 5.f }, 6);
    Debug<int>::LogLine(vct->length);
    for (int i = 0; i < vct->length; i++) 
    {
        Debug<int>::Log(i);
        Debug<const char*>::Log(": ");
        Debug<float>::LogLine(vct->vector[i]);
    }

    Renderable<ID2D1Bitmap*>* rd = new Renderable<ID2D1Bitmap*>("ID");
    

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