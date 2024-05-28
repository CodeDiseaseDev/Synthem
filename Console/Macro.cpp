#include "Macro.h"

Macro::Macro()
{
	control_name = "macro_control";

	AppendChild(&assignDrag);
	AppendChild(&enabledCheckBox);
	AppendChild(&valueRadialSlider);

	assignDrag.pTargetValue = &value;

	assignDrag.SetContent("*");
	valueRadialSlider.SetContent("");

	this->font.TextSize = 16;
	this->font.FontWeight = DWRITE_FONT_WEIGHT_BLACK;
	this->foreground = { 1,1,1,0.05 };

	outline_color = 0x1A73C7;
	background = 0x0f1113;

	this->cornerRadius = 4;
}

bool Macro::IsEnabled()
{
	return enabledCheckBox.checked;
}

void Macro::SetEnabled(bool enabled)
{
	enabledCheckBox.checked = enabled;
}

void Macro::update()
{
	if (enabledCheckBox.checked)
		value = valueRadialSlider.get_value();
	else
		value = 0;
}

void Macro::Paint(RenderTarget target)
{
	this->CalculateLayout();

	this->PaintBackground(target, true, IsEnabled());
	this->PaintChildren(target);

	if (IsEnabled())
	{
		auto brushBackground2 = this->CreateSolidBrush(target, ColorF{ 0xffffff,0.05 });

		float x = 0;
		float y = height * (1 - maths_utils::clamp(value,0,1));
		float _height = height;
		float _width = width;
		target->FillRectangle(
			{ x, y,_width,_height },
			brushBackground2);

		brushBackground2->Release();
	}

	this->PaintText(target);

	if (!IsEnabled()) {
		PaintDisabledRectangle(target);
	}
}

void Macro::CalculateLayout()
{
	float spacing = 10;

	assignDrag.cornerRadius = 15 / 2;
	assignDrag.width = 15;
	assignDrag.height = 15;

	const float centerY = height / 2;
	const float totalHeight = assignDrag.height + spacing + enabledCheckBox.height;

	assignDrag.location = {
		spacing,
		centerY - totalHeight / 2
	};

	enabledCheckBox.location = {
		assignDrag.location.x,
		assignDrag.location.y + assignDrag.height + spacing
	};

	valueRadialSlider.location = {
		enabledCheckBox.location.x + enabledCheckBox.width + spacing,
		height / 2 - valueRadialSlider.height / 2
	};

	width = valueRadialSlider.location.x +
		valueRadialSlider.width + spacing;
	height = valueRadialSlider.height + (spacing * 2);
}
