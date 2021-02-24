#pragma once

// Callum Mackenzie

// TODO : Make a linked list for the render queue

class Direct2DWindow
{
public:
    Direct2DWindow(RootWindow *window_);
    ~Direct2DWindow(); // Release render resources to avoid memory leaks
    void releaseResources(); // Release default style resources to avoid memory leaks
    ID2D1HwndRenderTarget *getRenderPane(); // Returns the render pane of this render window
    RootWindow *getWindow(); // Returns the RootWindow object of this Direct2DWindow
    void beginRender(); // Starts rendering
    void endRender(); // Ends rendering
    void drawQueue(bool preservePrev); // TODO : Draw render queue
    void drawRect(float x, float y, float width, float height, ID2D1SolidColorBrush *br);
    void drawBitmap(ID2D1Bitmap *bt, int width, int height, float top, float left, float rotation, float transparency, D2D1_POINT_2F rotationCenter, RECT sourceRect, D2D1_BITMAP_INTERPOLATION_MODE interpMode); // Draws bitmap
    int checkRenderQueueLength(); // TODO : Return length of render queue

public:
    HRESULT LoadFileBitmap(LPCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

public:
    RootWindow *window = nullptr; // RootWindow object linked to this window
    ID2D1Factory *pD2DFactory = nullptr; // Factory for creating the render surface
    ID2D1SolidColorBrush *pBlackBrush = nullptr; // Black colour brush
    ID2D1HwndRenderTarget *pRT = nullptr; // The render surface for the RootWindow
    ID2D1SolidColorBrush *varColPtr = nullptr; // A variable colour pointer
    D2D1::ColorF clearColour = D2D1::ColorF::White; // The colour that the screen clears to
    float zoom = 1.f; // The zoom of the render pane
    IDWriteFactory *m_pDWriteFactory = nullptr;
    IDWriteTextFormat *m_pTextFormat = nullptr;
};