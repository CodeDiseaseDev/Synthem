#include "Slider.h"

void Slider::Paint(RenderTarget target)
{
	/*float lineWidth = height;

	auto brush = this->CreateSolidBrush(
		target, background);

	target->DrawLine(
		{ 0,height / 2 },
		{ width,height / 2 },
		brush, lineWidth
	);*/

	this->cornerRadius = height / 2;
	this->PaintBackground(target, true);

	auto brush2 = this->CreateSolidBrush(
		target, foreground);

	auto brush1 = this->CreateSolidBrush(
		target, background);

	float r = height / 2 + 4;
	target->FillEllipse(
		{
			width * position,
			height / 2,
			r, r
		},
		brush2
	);

	target->DrawEllipse(
		{
			width * position,
			height / 2,
			r, r
		},
		brush1, 4
	);

	brush1->Release();
	brush2->Release();
}

void Slider::OffsetPos(float offset) {
	// clamp to 0 to 1
	position += offset;
	position = max(0, min(1, position));

	this->RepaintWindow();
}

void Slider::mouse_drag(int x, int y) {
	float offset = -x / this->width;

	OffsetPos(offset);
}
