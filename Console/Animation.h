#pragma once
#include "Control.h"
#include "Window.h"
#include <chrono>

using namespace std::chrono;

class Animation
{
public:
	Control* targetControl;
	bool started = false;

	float targetValue, renderValue, startValue;

	float duration = 0.5f;

	std::chrono::steady_clock::time_point start_point;

	Animation(Control* target)
	{
		this->targetControl = target;
	}

	void start()
	{
		started = true;

		startValue = renderValue;
		start_point = high_resolution_clock::now();

		targetControl->FindParentWindow()
			->StartAnimating();

	}

	float lerp(float a, float b, float f)
	{
		return a * (1.0 - f) + (b * f);
	}

	void start_if_not_started()
	{
		if (!started)
			start();
	}

	// called every frame
	void update()
	{
		if (!started)
			return;

		auto currentTime = std::chrono::duration<double>(
			std::chrono::high_resolution_clock::now() - start_point).count();

		float val = currentTime / duration;

		val = max(0, min(val, 1));
		val = val * val;

		if (val >= 1) {
			this->finish();
			return;
		}

		renderValue = lerp(startValue, targetValue, val);

	}

	void finish()
	{
		if (!started) return;
		started = false;

		targetControl->FindParentWindow()
			->StopAnimating();

		renderValue = targetValue;
	}

	void updated_values() {
		//startValue = renderValue;
	}
};
