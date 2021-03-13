#pragma once

// Callum Mackenzie

// TODO : Make a linked list for the render queue

namespace ingenium2D 
{
    class Direct2DWindow
    {
    public:
        Direct2DWindow(RootWindow* window_);
        ~Direct2DWindow(); // Release render resources to avoid memory leaks

        ID2D1HwndRenderTarget* getRenderPane(); // Returns the render pane of this render window
        RootWindow* getWindow(); // Returns the RootWindow object of this Direct2DWindow

        void resizePRT(UINT width, UINT height);
        void setSize(UINT wid, UINT hei);

        void beginRender(); // Starts rendering
        void endRender(); // Ends rendering
        void drawQueue(bool preservePrev);
        void drawBitmap(ID2D1Bitmap* bt, int width, int height, float top, float left, float rotX, float rotY, float rotZ, float transparency,
            D2D1_POINT_2F rotationCenter, RECT sourceRect, D2D1_BITMAP_INTERPOLATION_MODE interpMode,
            float scaleX, float scaleY); // Draws bitmap
        void drawRectangle(float x, float y, float width, float height, ID2D1Brush* brush, float strokeWidth = 1.f, ID2D1StrokeStyle* strokeStyle = (ID2D1StrokeStyle*)0);
        void drawEllipse(float elipseCenterX, float elipseCenterY, float elipseWidth, float elipseHeight, ID2D1Brush* brush, float strokeWidth = 1.f, ID2D1StrokeStyle* strokeStyle = (ID2D1StrokeStyle*)0);
        void drawLine(float point1X, float point1Y, float point2X, float point2Y, ID2D1Brush* brush, float strokeWidth = 1.f, ID2D1StrokeStyle* strokeStyle = (ID2D1StrokeStyle*)0);

        void calculateRPR();

        float getMouseX();
        float getMouseY();
        Vector2 getMousePos();


        template <typename T>
        inline void addToRenderQueue(T element, int type) {
            renderQueue->add(static_cast<void*>(element), type);
        };
        template <typename T>
        static inline void releaseID2D1Resource(T*& t) {
            if (t != nullptr) {
                t->Release();
            }
            t = NULL;
        };
    private:
        RECT getDesiredFrameRect(Renderable<ID2D1Bitmap>::FrameData fd, ID2D1Bitmap* rElement);

    public:
        RootWindow* window = nullptr; // RootWindow object linked to this window
        ID2D1Factory* pD2DFactory = nullptr; // Factory for creating the render surface
        ID2D1SolidColorBrush* pBlackBrush = nullptr; // Black colour brush
        ID2D1HwndRenderTarget* pRT = nullptr; // The render surface for the RootWindow
        ID2D1SolidColorBrush* varColPtr = nullptr; // A variable colour pointer
        D2D1::ColorF clearColour = D2D1::ColorF::White; // The colour that the screen clears to
        D2D1_SIZE_F zoom = D2D1::SizeF(1, 1); // The zoom of the render pane
        D2D1_POINT_2F offset = D2D1::Point2F(0, 0); // The offset of the render pane
        D2D1_MATRIX_3X2_F skew = D2D1::Matrix3x2F::Skew(0, 0, D2D1::Point2F(0, 0));
        IDWriteFactory* m_pDWriteFactory = nullptr;
        IDWriteTextFormat* m_pTextFormat = nullptr;

        float renderPixelRatio[2] = { 1, 1 };
        float aspectRatio[2] = { 16, 9 };

        struct RenderLinkedList : DynamicLinkedList
        {
            static const inline int TYPE_RENDER_ID2D1BITMAP = 1;
            static const inline int TYPE_RENDER_ID2D1TEXT = 2;
            static const inline int TYPE_RENDER_ID2D1LINE = 3;
            static const inline int TYPE_RENDER_SPRITE = 4;
            static const inline int TYPE_RENDER_VECTOR2 = 5;
        };

        RenderLinkedList* renderQueue = new RenderLinkedList(); // List of objects to render
    };
};