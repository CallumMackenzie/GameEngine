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
    void drawBitmap(ID2D1Bitmap *bt, int width, int height, float top, float left, float rotX, float rotY, float rotZ, float transparency, D2D1_POINT_2F rotationCenter, RECT sourceRect, D2D1_BITMAP_INTERPOLATION_MODE interpMode); // Draws bitmap
    template <typename T>
    inline void addToRenderQueue(T element, int type) {
        renderQueue->add(static_cast<void*>(element), type);
    };

public:
    HRESULT loadFileBitmap(LPCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

public:


    RootWindow *window = nullptr; // RootWindow object linked to this window
    ID2D1Factory *pD2DFactory = nullptr; // Factory for creating the render surface
    ID2D1SolidColorBrush *pBlackBrush = nullptr; // Black colour brush
    ID2D1HwndRenderTarget *pRT = nullptr; // The render surface for the RootWindow
    ID2D1SolidColorBrush *varColPtr = nullptr; // A variable colour pointer
    D2D1::ColorF clearColour = D2D1::ColorF::White; // The colour that the screen clears to
    D2D1_SIZE_F zoom = D2D1::SizeF(1, 1); // The zoom of the render pane
    D2D1_POINT_2F offset = D2D1::Point2F(0, 0); // The offset of the render pane
    D2D1_MATRIX_3X2_F skew = D2D1::Matrix3x2F::Skew(0, 0, D2D1::Point2F(0, 0));
    IDWriteFactory *m_pDWriteFactory = nullptr;
    IDWriteTextFormat *m_pTextFormat = nullptr;

    struct RenderLinkedList : DynamicLinkedList
    {
        static const inline int TYPE_RENDER_ID2D1BITMAP = 1;
        static const inline int TYPE_RENDER_ID2D1TEXT = 2;
        static const inline int TYPE_RENDER_ID2D1LINE = 3;
        static const inline int TYPE_RENDER_SPRITE = 4;
    };

    RenderLinkedList* renderQueue = new RenderLinkedList(); // List of objects to render
};