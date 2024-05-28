#include "CheckBox.h"

CheckBox::CheckBox()
{
	this->height = this->width = 15;
	this->control_name = "check_box";
	this->background = 0x05325b;
}

void CheckBox::CalculateLayout()
{
	// ensures ellipse appearance.
	this->cornerRadius = height / 2;
	width = height;
}

void CheckBox::Paint(RenderTarget target)
{
	this->PaintBackground(target, true, false);

	if (this->checked) {
		auto brush = this->CreateSolidBrush(target, 0x1e85e6);
		float space = 3;
		target->FillEllipse(
			{
				width / 2,
				height / 2,
				width / 2 - space,
				height / 2 - space
			},
			brush
		);
		brush->Release();
	}
}

void CheckBox::OnClick()
{
	this->checked = !this->checked;
	this->TryRepaintWindow();
}
