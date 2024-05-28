#include "RadialSlider.h"



double RadialSlider::get_value()
{
	value = theta;

	// add them together
	for (auto src : valueSources)
	{
		if (src != nullptr)
		{
			double v = *src;
			if (inner != nullptr)
				v *= inner->get_value();
			value += v;
		}
	}

	/*if (dTime != -1)
		value_eased = maths_utils::lerp(
			value_eased, value, dTimeStep * 200);*/

	//return value;
	return value;
}

double RadialSlider::get_value_range(double min, double max)
{
	return (get_value() * (max - min)) + min;
}

void RadialSlider::set_value_range(double min, double max, double value)
{
	theta = (value - min) / (max - min);
}

double RadialSlider::get_value_range_squared(double min, double max)
{
	double x = get_value_range(min, max);

	return x * x;
}

RadialSlider::RadialSlider(bool containsInner)
{
	width = 45;
	height = 45;
	cornerRadius = height / 2;
	background = 0x1E2126;
	outline_color = 0x888888;

	canHoldValue = true;

	this->control_name = "radial_slider";
	this->cursor = LoadCursor(NULL, IDC_SIZENS);

	this->font.FontWeight = DWRITE_FONT_WEIGHT_REGULAR;
	this->font.TextSize = 11;
	this->SetWContent(L"vol");

	if (containsInner) {
		inner = new RadialSlider(false);
		inner->SetWContent(L"");
		inner->theta = 1;
		AppendChild(inner);
	}
}

D2D1_POINT_2F RadialSlider::_InterpolateTheta(float theta, float r) {
	float x = (sin(theta) * r / 2) + r / 2;
	float y = (cos(theta) * r / 2) + r / 2;

	return { x,y };
}

void RadialSlider::Paint(RenderTarget target)
{
	CalculateLayout();
	//this->cornerRadius = height / 2;
	this->PaintBackground(target, true, false);


	this->value = get_value();

	

	float bottomSpacing = 45;
	float adjustedTheta = mapc(
		value, 0, 1,
		radians(360 - bottomSpacing),
		radians(bottomSpacing)
	);

	auto pos = _InterpolateTheta(adjustedTheta, width);

	D2D1_POINT_2F pos2 = { width / 2, height / 2 };
	
	auto brush = this->CreateSolidBrush(
		target,
		(value > 1) ? 0xff0000 : color
	);

	target->DrawLine(
		pos2, pos, brush, 3
	);

	float x = width / 2;
	float y = height / 2;
	float r = 4;

	target->FillEllipse(
		{ x,y,r,r }, brush);

	float translateY = height / 2 - 5;
	graphics_utils::translate(target, 0, translateY);
	this->PaintText(target);
	graphics_utils::translate(target, 0, -translateY);

	brush->Release();


	PaintChildren(target);
}

void RadialSlider::CalculateLayout()
{
	if (inner != nullptr) {
		inner->width = 15;
		inner->height = inner->width;
		inner->location = {
			width / 2 - inner->width / 2,
			height / 2 - inner->height / 2,
		};
	}
}

void RadialSlider::mouse_drag(int x, int y) {
	// calculate total x + y mouse movement (0-1)
	float z = -x + y;

	// incr/decr angle to z
	this->theta += z / this->height * .1;

	this->theta = max(this->theta, 0);
	this->theta = min(this->theta, 1);


	this->RepaintWindow();
}

//void RadialSlider::Set_pValue(float value)
//{
//	this->theta = map(value, 0, 1, radians(0), radians(360));
//}
