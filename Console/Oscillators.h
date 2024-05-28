#pragma once
#include <corecrt_math.h>
#include <cstdlib>

//constexpr double PI = 3.14159265359;

constexpr auto _____OSC_SHAPE_ENUM_LINE = __LINE__;
enum OSC_SHAPE {
	OSC_SINE,
	OSC_SQUARE,
	OSC_TRIANGLE,
	OSC_SAW_ANA,
	OSC_NOISE,
	OSC_SAMPLE
};
constexpr auto OSC_SHAPE_ITEMS = __LINE__ - _____OSC_SHAPE_ENUM_LINE - 3;

inline inline double w(double dHertz)
{
	return dHertz * 2.0 * PI;
}

inline double osc(double phase, OSC_SHAPE nType = OSC_SINE)
{
	switch (nType)
	{
	case OSC_SINE: // Sine wave bewteen -1 and +1
		return sin(phase);

	case OSC_SQUARE: // Square wave between -1 and +1
		return sin(phase) > 0 ? 1.0 : -1.0;

	case OSC_TRIANGLE: // Triangle wave between -1 and +1
		return asin(sin(phase)) * (2.0 / PI);

	case OSC_SAW_ANA: // Saw wave (analogue / warm / slow)
	{
		double dOutput = 0.0;

		for (double n = 1.0; n < 40.0; n++)
			dOutput += (sin(n * phase)) / n;

		return dOutput * (2.0 / PI);
	}



	case OSC_NOISE: // Pseudorandom noise
		return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;

	case OSC_SAMPLE:
		return 0.0;

	default:
		return 0.0;
	}
}