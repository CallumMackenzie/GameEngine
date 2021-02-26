#include "ModWin.h"
#include "WindowClass.h"
#include "Window.h"
#include "Log.h"
#include "Direct2D.h"
#include "C_WICImageFactory.h"
#include "LinkedList.h"
#include "Bitmap.h"
#include "Renderable.h"
#include "Sprite.h"
#include "Direct2DWindow.h"

// Callum Mackenzie

Direct2DWindow::Direct2DWindow(RootWindow *window_)
{
    window = window_;
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory);

    RECT rc;
    GetClientRect(window->getHWND(), &rc);

    hr = pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            window->getHWND(),
            D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top),
            D2D1_PRESENT_OPTIONS_NONE),
        &pRT);

    if (SUCCEEDED(hr))
    {
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &pBlackBrush);
    }

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(m_pDWriteFactory),
        reinterpret_cast<IUnknown **>(&m_pDWriteFactory));
    if (SUCCEEDED(hr))
    {
        hr = m_pDWriteFactory->CreateTextFormat(
            L"Verdana",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"",
            &m_pTextFormat);
    }

    if (SUCCEEDED(hr))
    {
        m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }
    // https://docs.microsoft.com/en-us/windows/win32/direct2d/getting-started-with-direct2d#step-1-include-direct2d-header
}

Direct2DWindow::~Direct2DWindow()
{
    if (renderQueue) {
        delete renderQueue;
    }
    if (pBlackBrush) {
        pBlackBrush->Release();
        pBlackBrush = NULL;
    }
    if (varColPtr) {
        varColPtr->Release();
        pBlackBrush = NULL;
    }
    if (m_pTextFormat) {
        m_pTextFormat->Release();
        pBlackBrush = NULL;
    }
    if (pD2DFactory) {
        pD2DFactory->Release();
        pBlackBrush = NULL;
    }
    if (m_pDWriteFactory) {
        m_pDWriteFactory->Release();
        pBlackBrush = NULL;
    }
    if (pRT) {
        pRT->Release();
        pRT = NULL;
    }
    if (window != nullptr) {
        delete window;
        window = nullptr;
    }
}

void Direct2DWindow::releaseResources()
{
    if (pRT) {
        pRT->Release();
        pRT = NULL;
    }
    if (pBlackBrush) {
        pBlackBrush->Release();
        pBlackBrush = NULL;
    }
}

ID2D1HwndRenderTarget *Direct2DWindow::getRenderPane()
{
    return pRT;
}
RootWindow *Direct2DWindow::getWindow()
{
    return window;
}
void Direct2DWindow::beginRender()
{
    pRT->BeginDraw();
}
void Direct2DWindow::endRender()
{
    pRT->EndDraw();
}
void Direct2DWindow::drawQueue(bool preservePrev)
{
    if (pRT != nullptr)
    {
        pRT->SetTransform(D2D1::Matrix3x2F::Rotation(0, D2D1::Point2F(0, 0)));
        if (!preservePrev)
        {
            pRT->Clear(clearColour);
        }
        pRT->SetTransform(D2D1::Matrix3x2F::Scale(zoom, D2D1::Point2F(0, 0)));
        pRT->SetTransform(D2D1::Matrix3x2F::Translation(offset.x, offset.y));
        pRT->SetTransform(skew);
        RenderLinkedList::Node* node = renderQueue->head;
        while (node != nullptr) 
        {
            switch (node->type) 
            {
            case RenderLinkedList::TYPE_RENDER_ID2D1BITMAP:
                try {
                    Renderable<ID2D1Bitmap>* rObj = ((Renderable<ID2D1Bitmap>*)node->data);
                    if (rObj->renderElement == nullptr) {
                        break;
                    }
                    RECT rct = RECT();
                    rct.left = 0;
                    rct.top = 0;
                    rct.bottom = rObj->renderElement->GetSize().height;
                    rct.right = rObj->renderElement->GetSize().width;
                    drawBitmap(rObj->renderElement, rObj->renderElement->GetSize().width,
                        rObj->renderElement->GetSize().width, rObj->position->x(), rObj->position->y(), rObj->rotation->x, rObj->rotation->y, rObj->rotation->z, rObj->transparency,
                        D2D1::Point2F(rObj->rotation->centre[0], rObj->rotation->centre[1]), rct, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
                }
                catch (std::exception& e) {
                    e.what();
                }
                break;
            case RenderLinkedList::TYPE_RENDER_ID2D1LINE:
                // TODO: Render line
                break;
            case RenderLinkedList::TYPE_RENDER_ID2D1TEXT:
                // TODO: Render text
                break;
            case RenderLinkedList::TYPE_RENDER_SPRITE:
                // TODO: Render sprite
                break;
            default:
                Debug::oss << "Unrecognized type: " << node->type << ", not drawing.";
                Debug::writeLn();
                break;
            }
            node = node->next;
        }
        renderQueue = new RenderLinkedList();
    }
}
void Direct2DWindow::drawRect(float x, float y, float width, float height, ID2D1SolidColorBrush *br)
{
    pRT->DrawRectangle(
        D2D1::RectF(
            x,
            y,
            x + width,
            x + height),
        br, 10.0f);
}
void Direct2DWindow::drawBitmap(ID2D1Bitmap *bt, int width, int height, float top, float left, float rotX, float rotY, float rotZ, float transparency, D2D1_POINT_2F rotationCenter, RECT sourceRect, D2D1_BITMAP_INTERPOLATION_MODE interpMode)
{
    D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(top, left);
    pRT->SetTransform(D2D1::Matrix3x2F::Rotation(rotZ, rotationCenter));
    pRT->DrawBitmap(
        bt,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + width,
            upperLeftCorner.y + height),
        transparency,
        interpMode,
        D2D1::RectF(
            (float)sourceRect.left,
            (float)sourceRect.top,
            (float)sourceRect.right,
            (float)sourceRect.bottom));
}

HRESULT Direct2DWindow::loadFileBitmap(LPCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
    IWICImagingFactory* pIWICFactory = C_WICImagingFactory::GetWIC();
    IWICBitmapDecoder* pDecoder = NULL;
    IWICBitmapFrameDecode* pSource = NULL;
    IWICStream* pStream = NULL;
    IWICFormatConverter* pConverter = NULL;
    IWICBitmapScaler* pScaler = NULL;

    HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
        uri,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );
    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pSource);
    }

    if (SUCCEEDED(hr))
    {

        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = pIWICFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pSource,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut
        );
    }

    if (SUCCEEDED(hr))
    {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = pRT->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            ppBitmap
        );
    }

    if (pDecoder) {
        pDecoder->Release();
        pDecoder = NULL;
    }
    if (pSource) {
        pSource->Release();
        pSource = NULL;
    }
    if (pStream) {
        pStream->Release();
        pStream = NULL;
    }
    if (pConverter) {
        pConverter->Release();
        pConverter = NULL;
    }
    if (pScaler) {
        pScaler->Release();
        pScaler = NULL;
    }
    return hr;
}
