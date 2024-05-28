#include "LFOGraphControl.h"
#include <algorithm>
#include "utils.h"

#define DISTANCE(ax, ay, bx, by)\
	sqrt(pow(bx - ax, 2) + \
	pow(by - ay, 2) * 1.0)



void LFOGraphControl::Paint(RenderTarget target)
{
	CalculateLayout();

	this->PaintBackground(target, true, IsEnabled());
	auto brush = this->CreateSolidBrush(target, 0x1A73C7);
	auto brush2 = this->CreateSolidBrush(target, 0xC71A73);
	auto brushBackground2 = this->CreateSolidBrush(target, ColorF{0xffffff,0.05});


	PaintGrid(target);

	float x = 0;
	float y = height * (1 - value);
	float _height = height;
	float _width = width;
	target->FillRectangle(
		{ x, y,_width,_height },
		brushBackground2);


	this->PaintText(target);

	for (int i = 1; i < this->Points.size(); i++) {
		target->DrawLine(
			{ this->Points[i - 1].x * width, this->Points[i - 1].y * height },
			{ this->Points[i].x * width, this->Points[i].y * height },
			brush, 2
		);
	}

	for (int i = 0; i < this->Points.size(); i++) {
		target->FillEllipse(
			{
				this->Points[i].x * width,
				this->Points[i].y * height,
				4, 4
			},
			brush
		);
	}

	int index = HitTestPoints();
	if (index != -1)
	{
		D2D1_POINT_2F point = this->Points[index];
		if (point.x != -1 && point.y != -1) {
			target->FillEllipse(
				{
					point.x * width,
					point.y * height,
					7, 7 
				},
				brush
			);
		}
	}

	if (IsEnabled()) {
		float _y = InterpolateY(xValue);
		_y = 1 - _y;
		float lineLength = 5;
		target->DrawLine(
			{ (float)xValue * width, _y * height - lineLength },
			{ (float)xValue * width, _y * height + lineLength },
			brush2, 3
		);

	}

	this->PaintChildren(target);

	if (!IsEnabled()) {
		PaintDisabledRectangle(target);
	}

	brush->Release();
	brush2->Release();
	brushBackground2->Release();
}

int LFOGraphControl::HitTestPoints() {
	for (int i = 0; i < this->Points.size(); i++) {
		float cx = this->mouse_hover_location.x;
		//float cy = InterpolateY(cx) * height;
		float cy = this->mouse_hover_location.y;

		float px = this->Points[i].x * width;
		float py = this->Points[i].y * height;

		float dist = DISTANCE(cx, cy, px, py);

		if (dist < 7) return i;
	}

	return -1;
}

void LFOGraphControl::PaintGrid(RenderTarget target)
{
	auto brush = this->CreateSolidBrush(target, {.25,.25,.25,.5});

	float xLineCount = 8;
	float yLineCount = 8;
	float xStep = width / xLineCount;
	float yStep = height / yLineCount;

	for (int x = 0; x < xLineCount; x++) {
		target->DrawLine(
			{ xStep * x, 0 },
			{ xStep * x, height },
			brush
		);
	}

	for (int y = 0; y < yLineCount; y++) {
		target->DrawLine(
			{ 0, yStep * y },
			{ width, yStep * y },
			brush
		);
	}

	brush->Release();
}

void LFOGraphControl::OnDoubleClick(int x, int y)
{
	int index = HitTestPoints();

	if (index != -1)
	{
		if (index != 0 && index != this->Points.size() - 1)
		{
			this->Points.erase(
				std::next(this->Points.begin(), index)
			);
			RepaintWindow();
		}
		return;
	}

  AddPoint((float)x / width, (float)y / height);
	this->RepaintWindow();
	point_drag_index = this->Points.size() - 1;
}

void LFOGraphControl::AddPoint(float x, float y)
{
  this->Points.push_back({ x, y });

	std::sort(Points.begin(), Points.end(),
		[](const D2D1_POINT_2F& a, const D2D1_POINT_2F& b) {
		return a.x < b.x;
	});
}

double LFOGraphControl::InterpolateY(double x)
{
	if (!IsEnabled())
		return 0;

	xValue = x;

	if (Points.empty())
		return 0;

	if (x >= Points.back().x)
		return 1 - Points.back().y;

	if (x <= Points.front().x)
		return 1 - Points.front().y;

	for (size_t i = 0; i < Points.size() - 1; ++i) {
		if (x >= Points[i].x && x <= Points[i + 1].x) {
			double x1 = Points[i].x;
			double y1 = Points[i].y;
			double x2 = Points[i + 1].x;
			double y2 = Points[i + 1].y;
			return 1 - (y1 + (x - x1) * (y2 - y1) / (x2 - x1));
		}
	}

	return 0;
}

void LFOGraphControl::AddPointInverseY(float x, float y)
{
  AddPoint(x, 1 - y);
}

void LFOGraphControl::mouse_move(int x, int y)
{
	RepaintWindow();
}

void LFOGraphControl::mouse_down()
{
	int index = HitTestPoints();

	if (index == 0 || index == this->Points.size() - 1)
		return;

	if (index != -1)
	{
		D2D1_POINT_2F point = this->Points[index];

		if (point.x != -1 && point.y != -1) {
			point_drag_index = index;
		}
	}
}

void LFOGraphControl::mouse_up()
{
	point_drag_index = -1;
}

float LFOGraphControl::GetMinXForPoint(int index) {
	if (index == 0) return 0;
	return this->Points[index - 1].x;
}

float LFOGraphControl::GetMaxXForPoint(int index) {
	if (index == this->Points.size() - 1) return 1;
	return this->Points[index + 1].x;
}

void LFOGraphControl::update(double dTime, double bpm)
{
	double x = maths_utils::beats_to_lfo_pos(
		maths_utils::time_to_beats(dTime, bpm) / lfoBpm.get_bpm_value());

	this->value = InterpolateY(x);
	this->xValue = x;
}

void LFOGraphControl::mouse_drag(int x, int y)
{
	if (point_drag_index != -1) {
		Points[point_drag_index].x += -x / width;
		Points[point_drag_index].x = maths_utils::clamp(
			Points[point_drag_index].x,
			GetMinXForPoint(point_drag_index),
			GetMaxXForPoint(point_drag_index)
		);

		this->Points[point_drag_index].y += -y / height;
		this->Points[point_drag_index].y = maths_utils::clamp(
			this->Points[point_drag_index].y,
			0, 1
		);
		RepaintWindow();
	}
}



void LFOGraphControl::CalculateLayout()
{
	drag.width = 15;
	drag.height = 15;
	drag.cornerRadius = 15 / 2;
	drag.location = { 10, 10 };

	enabledCheckBox.location = {
		width - enabledCheckBox.width - 10,
		10
	};

	lfoBpm.width = 25;
	lfoBpm.height = 25;

	lfoBpm.location = {
		width / 2 - lfoBpm.width / 2,
		10
	};

	lfoBpm.SetMode(BPMRadialSliderMode_Divisor);
}

bool LFOGraphControl::IsEnabled()
{
	return enabledCheckBox.checked;
}

void LFOGraphControl::SetEnabled(bool enabled)
{
	enabledCheckBox.checked = enabled;
}