#include "WaveShapeRenderer.h"

double WaveShapeRenderer::get_y_point(float x)
{
	double space = 5;
	double px = (x / width) * PI * 2;
	return map(
		osc(px, oscillator_shape),
		-1.0, 1.0,
		space + 1, height - space
	);
}

WaveShapeRenderer::WaveShapeRenderer()
{
	this->control_name = "shape_renderer";

	this->width = 100;
	this->height = 45;
	this->cornerRadius = 4;

	background = 0x1D2126;
}

void WaveShapeRenderer::Paint(RenderTarget target)
{
	this->PaintBackground(target, true, false);

	auto brush = this->CreateSolidBrush(
		target, 0x6F63B1);

	for (float x = 1; x < width; x++)
	{
		double lastY = get_y_point(x - 1);
		double thisY = get_y_point(x);

		target->DrawLine(
			{ x - 1, (float)lastY },
			{ x,		 (float)thisY },
			brush, 1
		);
	}

	brush->Release();
}

void WaveShapeRenderer::OnClick()
{
	int next_shape = (int)oscillator_shape + 1;
	if (next_shape >= OSC_SHAPE_ITEMS)
	{
		next_shape = 0;
	}

	this->oscillator_shape = (OSC_SHAPE)next_shape;

	if (_onClick != nullptr)
	{
		_onClick();
	}
}

void WaveShapeRenderer::SetOnClickCallback(std::function<void()> func)
{
	_onClick = func;
}
