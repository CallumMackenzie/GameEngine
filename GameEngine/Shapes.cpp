#include "Vector2.h"
#include "Renderable.h"
#include "Shapes.h"

using namespace ingenium2D;

Line::Line(Vector2 pos, Vector2 end_, ID2D1StrokeStyle* pStrokeStyle) : Renderable<ID2D1StrokeStyle>("LINE")
{
	renderElement = pStrokeStyle;
	position = pos;
	end = end_;
}

Line::~Line()
{
	if (renderElement) {
		renderElement->Release();
		renderElement = NULL;
	}
	if (brush) {
		brush->Release();
		brush = NULL;
	}
}
