#pragma once
#include "Control.h"

class ScrollBar : public Control
{
public:
	float trackbarSize = 0.15f;
	float trackbarPos = 0.0f;

	float trackbarMargin = 6;

	Orientation orientation = VERTICAL;

	inline bool is_horizontal() {
		return this->orientation == HORIZONTAL;
	}

	ScrollBar() {
		this->width = 15;
		this->height = 150;

		this->background = 0x0f1113;
		this->outline_color = 0x1A73C7;
	}

	void Paint(RenderTarget target) override;
	void mouse_drag(int x, int y) override;
	void mouse_wheel(int delta) override;

	void OffsetPos(float offset);

	void SetOrientation(Orientation orientation);
};

