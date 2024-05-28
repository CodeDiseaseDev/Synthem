#pragma once
#include "Control.h"
#include "Oscillators.h"
#include <functional>

class WaveShapeRenderer :
	public Control
{
private:
	double get_y_point(float x);

	std::function<void()> _onClick;

public:
	WaveShapeRenderer();

	OSC_SHAPE oscillator_shape = OSC_SINE;

	void Paint(RenderTarget target) override;

	void OnClick() override;
	void SetOnClickCallback(std::function<void()> func);
};

