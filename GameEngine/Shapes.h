#pragma once

namespace ingenium2D {
	struct Line : Renderable<ID2D1StrokeStyle> {
		Line(Vector2 pos, Vector2 end_, ID2D1StrokeStyle* pStrokeStyle = (ID2D1StrokeStyle*) 0);
		~Line();

		ID2D1SolidColorBrush* brush = nullptr;
		Vector2 end = Vector2();
		float lineSize = 1.f;
	};
};