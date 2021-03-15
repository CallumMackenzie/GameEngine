#include "ModWin.h"
#include "Memory.h"
#include "WindowClass.h"
#include "Window.h"
#include "Log.h"
#include "Direct2D.h"
#include "C_WICImageFactory.h"
#include "LinkedList.h"
#include "Renderable.h"
#include "Shapes.h"
#include "Sprite.h"
#include "Input.h"
#include "Direct2DWindow.h"

using namespace ingenium2D;

// Callum Mackenzie

Direct2DWindow::Direct2DWindow(RootWindow* window_)
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
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
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
	calculateRPR();
}
Direct2DWindow::~Direct2DWindow()
{
	memory::safe_delete(renderQueue);

	releaseID2D1Resource(pBlackBrush);
	releaseID2D1Resource(varColPtr);
	releaseID2D1Resource(m_pTextFormat);
	releaseID2D1Resource(pD2DFactory);
	releaseID2D1Resource(m_pDWriteFactory);
	releaseID2D1Resource(pRT);

	memory::safe_delete(window);
}
ID2D1HwndRenderTarget* Direct2DWindow::getRenderPane()
{
	return pRT;
}
RootWindow* Direct2DWindow::getWindow()
{
	return window;
}
void Direct2DWindow::resizePRT(UINT width, UINT height)
{
	if (pRT) {
		D2D1_SIZE_U size;
		size.width = width;
		size.height = height;
		pRT->Resize(size);
	}
}
void Direct2DWindow::setSize(UINT wid, UINT hei)
{
	window->setSize(wid, hei);
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
		if (!preservePrev)
		{
			pRT->Clear(clearColour);
		}
		RenderLinkedList::Node* node = renderQueue->head;
		while (node != nullptr)
		{
			pRT->SetTransform(D2D1::Matrix3x2F::Rotation(0, D2D1::Point2F(0, 0)));
			pRT->SetTransform(skew);
			switch (node->type)
			{
			case RenderLinkedList::TYPE_RENDER_ID2D1BITMAP:
			{
				{
					Renderable<ID2D1Bitmap>* rObj = ((Renderable<ID2D1Bitmap>*)node->data);
					if (rObj->renderElement == nullptr)
						break;
					RECT rct = getDesiredFrameRect(rObj->frameData, rObj->renderElement);
					drawBitmap(rObj->renderElement, // Bitmap
						rObj->frameData.frameWidth, // Width
						rObj->frameData.frameHeight, // Height
						rObj->position.x, // Left
						rObj->position.y, // Top
						rObj->rotation.x, rObj->rotation.y, rObj->rotation.z, // Rotation
						rObj->transparency, // Transparency
						D2D1::Point2F(rObj->rotation.centre[0] + rObj->position.x, rObj->rotation.centre[1] + rObj->position.y), // Center of rotation
						rct, // Bitmap source rect
						D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, // Interpolation mode
						rObj->scale.x, rObj->scale.y); // Scale
				}
				break;
			}
			case RenderLinkedList::TYPE_RENDER_ID2D1LINE:
			{
				Line* rObj = ((Line*)node->data);
				drawLine(rObj->position.x, rObj->position.y,
					rObj->end.x, rObj->end.y,
					(rObj->brush == nullptr) ? pBlackBrush : rObj->brush,
					rObj->lineSize,
					rObj->renderElement);
			}
			break;
			case RenderLinkedList::TYPE_RENDER_ID2D1TEXT:
				// TODO: Render text
				break;
			case RenderLinkedList::TYPE_RENDER_SPRITE:
			{
				Sprite* rObj = ((Sprite*)node->data);
				if (rObj->renderHitbox) {
					if (rObj->hitbox2D.type == Hitbox2D::TYPE_RECTANGLE) {
						drawRectangle(
							rObj->hitbox2D.rectPos().x,
							rObj->hitbox2D.rectPos().y,
							rObj->hitbox2D.rectSize().x,
							rObj->hitbox2D.rectSize().y,
							pBlackBrush
						);
					}
					else if (rObj->hitbox2D.type == Hitbox2D::TYPE_CIRCLE) {
						drawEllipse(rObj->hitbox2D.circleCentre().x, rObj->hitbox2D.circleCentre().y, rObj->hitbox2D.circleRadius(), rObj->hitbox2D.circleRadius(), pBlackBrush);
					}
				}
				if (rObj->renderElement == nullptr) {
					break;
				}
				RECT rct = getDesiredFrameRect(rObj->frameData, rObj->renderElement);
				drawBitmap(
					rObj->renderElement, // Bitmap
					rObj->size.x, // Width
					rObj->size.y, // Height
					rObj->position.x, // Top
					rObj->position.y, // Left
					rObj->rotation.x, rObj->rotation.y, rObj->rotation.z, // Rotation
					rObj->transparency, // Transparency
					D2D1::Point2F(rObj->rotation.centre[0] + rObj->position.x, rObj->rotation.centre[1] + rObj->position.y), // Rotation center
					rct, // Bitmap source rect
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, // Bitmap interpolation mode
					rObj->scale.x, rObj->scale.y // Scale
				);
			}
			break;
			default:
				break;
			}
			node = node->next;
		}
		memory::safe_delete(renderQueue);
		renderQueue = new RenderLinkedList();
	}
}
void Direct2DWindow::drawBitmap(ID2D1Bitmap* bt, int width, int height, float top, float left, float rotX, float rotY, float rotZ, float transparency,
	D2D1_POINT_2F rotationCenter, RECT sourceRect, D2D1_BITMAP_INTERPOLATION_MODE interpMode,
	float scaleX, float scaleY)
{
	rotationCenter.x = (rotationCenter.x + offset.x) * zoom.width * renderPixelRatio[0];
	rotationCenter.y = (rotationCenter.y + offset.y) * zoom.height * renderPixelRatio[1];
	D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(top, left);
	upperLeftCorner.x += offset.x;
	upperLeftCorner.y += offset.y;
	pRT->SetTransform(D2D1::Matrix3x2F::Rotation(rotZ, rotationCenter));
	pRT->DrawBitmap(
		bt,
		D2D1::RectF(
			upperLeftCorner.x * zoom.width * renderPixelRatio[0],
			upperLeftCorner.y * zoom.height * renderPixelRatio[1],
			(upperLeftCorner.x + (width * scaleX)) * zoom.width * renderPixelRatio[0],
			(upperLeftCorner.y + (height * scaleY)) * zoom.width * renderPixelRatio[1]),
		transparency,
		interpMode,
		D2D1::RectF(
			(float)sourceRect.left,
			(float)sourceRect.top,
			(float)sourceRect.right,
			(float)sourceRect.bottom));
}
void Direct2DWindow::drawRectangle(float x, float y, float width, float height, ID2D1Brush* brush, float strokeWidth, ID2D1StrokeStyle* strokeStyle)
{
	x += offset.x;
	y += offset.y;
	pRT->DrawRectangle(D2D1::RectF(
		x * zoom.width * renderPixelRatio[0],
		y * zoom.height * renderPixelRatio[1],
		(x + width) * zoom.width * renderPixelRatio[0],
		(y + height) * zoom.width * renderPixelRatio[1]),
		brush, strokeWidth * zoom.width * renderPixelRatio[0],
		strokeStyle);
}
void Direct2DWindow::drawEllipse(float elipseCenterX, float elipseCenterY, float elipseWidth, float elipseHeight, ID2D1Brush* brush, float strokeWidth, ID2D1StrokeStyle* strokeStyle)
{
	elipseCenterX += offset.x;
	elipseCenterY += offset.y;
	pRT->DrawEllipse(D2D1::Ellipse(
		D2D1::Point2F(
			elipseCenterX * zoom.width * renderPixelRatio[0],
			elipseCenterY * zoom.height * renderPixelRatio[1]),
		elipseWidth * zoom.width * renderPixelRatio[0],
		elipseHeight * zoom.height * renderPixelRatio[1]
	), brush, strokeWidth * zoom.width * renderPixelRatio[0], strokeStyle);
}
void Direct2DWindow::drawLine(float point1X, float point1Y, float point2X, float point2Y, ID2D1Brush* brush, float strokeWidth, ID2D1StrokeStyle* strokeStyle)
{
	point1X += offset.x;
	point2X += offset.x;
	point1Y += offset.y;
	point2Y += offset.y;
	pRT->DrawLine(
		D2D1::Point2F(point1X * zoom.width * renderPixelRatio[0], point1Y * zoom.height * renderPixelRatio[1]),
		D2D1::Point2F(point2X * zoom.width * renderPixelRatio[0], point2Y * zoom.height * renderPixelRatio[1]),
		brush,
		strokeWidth,
		strokeStyle);
}
void ingenium2D::Direct2DWindow::drawTriangle(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y, ID2D1Brush* brush, float strokeWidth, ID2D1StrokeStyle* strokeStyle)
{
	drawLine(point1X, point1Y, point2X, point2Y, brush, strokeWidth, strokeStyle);
	drawLine(point2X, point2Y, point3X, point3Y, brush, strokeWidth, strokeStyle);
	drawLine(point3X, point3Y, point1X, point1Y, brush, strokeWidth, strokeStyle);
}
void Direct2DWindow::calculateRPR()
{
	RECT* rect = new RECT();
	float aspect_1_x = aspectRatio[0] / aspectRatio[1];
	GetWindowRect(getWindow()->getHWND(), rect);
	float win_width = rect->right - rect->left;
	float win_height = rect->bottom - rect->top;

	renderPixelRatio[0] = win_width / (aspectRatio[0] * 100);
	renderPixelRatio[1] = win_height / (aspectRatio[1] * 100);

	memory::safe_delete(rect);
}
float Direct2DWindow::getMouseX()
{
	return Input::getInput()->getHWNDCursorPos(window->getHWND()).x / renderPixelRatio[0];
}
float Direct2DWindow::getMouseY()
{
	return Input::getInput()->getHWNDCursorPos(window->getHWND()).y / renderPixelRatio[1];
}
Vector2 Direct2DWindow::getMousePos()
{
	Vector2 v2 = Input::getInput()->getHWNDCursorPos(window->getHWND());
	v2.divide(renderPixelRatio[0], renderPixelRatio[1]);
	return v2;
}
float ingenium2D::Direct2DWindow::screenWidth()
{
	return aspectRatio[0] * 100;
}
float ingenium2D::Direct2DWindow::screenHeight()
{
	return aspectRatio[1] * 100;
}
int ingenium2D::Direct2DWindow::screenWidthPixels()
{
	RECT* rect = new RECT();
	GetWindowRect(getWindow()->getHWND(), rect);
	float win_width = rect->right - rect->left;
	memory::safe_delete(rect);
	return win_width;
}
int ingenium2D::Direct2DWindow::screenHeightPixels()
{
	RECT* rect = new RECT();
	GetWindowRect(getWindow()->getHWND(), rect);
	float win_height = rect->bottom - rect->top;	
	memory::safe_delete(rect);
	return win_height;
}
RECT ingenium2D::Direct2DWindow::getDesiredFrameRect(Renderable<ID2D1Bitmap>::FrameData fd, ID2D1Bitmap* rElement)
{
	RECT rct = RECT();
	if (fd.frames <= 1) { // Static image
		rct.left = fd.startFrame[0] * fd.frameWidth;
		rct.top = fd.startFrame[1] * fd.frameHeight;
		if (fd.frameWidth > 0) {
			rct.right = (fd.startFrame[0] + 1) * fd.frameWidth;
		}
		else {
			rct.right = rElement->GetSize().width;
		}
		if (fd.frameHeight > 0) {
			rct.bottom = (fd.startFrame[1] + 1) * fd.frameHeight;
		}
		else {
			rct.bottom = rElement->GetSize().height;
		}
	}
	else {
		if (fd.spriteSheetDirection) { // Vertical
			rct.left = fd.startFrame[0] * fd.frameWidth;
			rct.right = (fd.startFrame[0] + 1) * fd.frameWidth;

			rct.top = fd.frameHeight * fd.frame;
			rct.bottom = fd.frameHeight * (fd.frame + 1);
		}
		else { // Horizontal
			rct.left = fd.frameWidth * fd.frame;
			rct.right = fd.frameWidth * (fd.frame + 1);

			rct.top = fd.startFrame[1] * fd.frameHeight;
			rct.bottom = (fd.startFrame[1] + 1) * fd.frameHeight;
		}
	}
	return rct;
}
