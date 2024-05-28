#include "AudioVisualizer.h"
#include <iostream>
#include <limits>

#include "SoundMaker.h"

float lerp(float a, float b, float f)
{
	float x = a * (1.0 - f) + (b * f);
	if (x == NAN) return 0;
	return x;
}

void AudioVisualizer::Paint(RenderTarget target)
{

	this->PaintBackground(target, true, true);
	//return;
	auto brush = CreateSolidBrush(target, 0x1A73C7);
	auto brushBackground = CreateSolidBrush(target, ColorF{ 0xffffff,0.1 });

	float x = 0;
	float y = height * (1 - equalizer_value);
	float _height = height;
	float _width = width;

	target->FillRoundedRectangle(
		RoundedRect(
			RectF(x, y, _width, _height),
			cornerRadius, cornerRadius
		),
		brushBackground
	);

	auto& vec = soundMaker->audio_buffer;
	float resolution = 1.0f; // skip samples

	float spacing = 8;

	//float x_last = 0, y_last = (vec[0] / SHRT_MAX / 2) + .5;

	float avg = 0;

	for (float i = 1; i < vec.size(); i += 1 / resolution) {
		if (i >= vec.size())
			break;

		//debug_printf("i: %.2f, size: %i\n", i - (1 / resolution), vec.size());

		float x = i / vec.size();
		//float x_last = (float)(i - 1) / vec.size();
		float x_last = (i - (1 / resolution)) / vec.size();
		float y_last = (vec[i - (1 / resolution)] / SHRT_MAX / 2) + .5;
		float y = (vec[i] / SHRT_MAX / 2) + .5;

		float w = width - (spacing * 2);
		float h = height - (spacing * 2);

		avg += abs(vec[i]);

		target->DrawLine(
			{ x_last * w + spacing, y_last * h + spacing },
			{ x * w + spacing, y * h + spacing },
			brush, 2
		);
	}

	avg /= (float)vec.size();
	if (avg == NAN)
	{
		debug_printf("/= size\n");

	}
	avg /= SHRT_MAX;
	if (avg == NAN)
	{
		debug_printf("/= SHRT_MAX\n");

	}
	avg = maths_utils::clamp(avg, 0, 1);
	if (avg == NAN)
	{
		debug_printf("maths_utils::clamp\n");

	}

	equalizer_value = lerp(equalizer_value, avg, 0.1);
	//equalizer_value = avg;
	//avg = abs(avg);

	//debug_printf("%.2f\n", equalizer_value);

	

	brush->Release();
	brushBackground->Release();
}

AudioVisualizer::AudioVisualizer() {
	background = 0x0f1113;
	outline_color = 0x1A73C7;

	width = 100;
	height = 50;

	cornerRadius = 6;
}
