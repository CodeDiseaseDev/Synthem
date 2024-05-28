#pragma once
#include "RadialSlider.h"

enum BPMRadialSliderMode {
	BPMRadialSliderMode_Divisor,
	BPMRadialSliderMode_PerMinute,
	BPMRadialSliderMode_BeatCount
};

class BPMRadialSlider : public RadialSlider
{
public:
	BPMRadialSliderMode mode = BPMRadialSliderMode_PerMinute;

	void SetMode(BPMRadialSliderMode mode);

	BPMRadialSlider();

	double GetLFOSpeed(double mult);
	std::wstring GetLFOSpeedString(double mult);

	double get_bpm_value();
};

