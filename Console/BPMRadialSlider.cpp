#include "BPMRadialSlider.h"

void BPMRadialSlider::SetMode(BPMRadialSliderMode mode)
{
	this->mode = mode;
}

BPMRadialSlider::BPMRadialSlider() :
	RadialSlider(false)
{

}

double BPMRadialSlider::GetLFOSpeed(double mult)
{
	for (int i = 32; i >= 2; i /= 2)
	{
		if (mult <= 1.0 / i)
			return 1.0 / i;
	}

	return (int)(mult * 2);
}

std::wstring BPMRadialSlider::GetLFOSpeedString(double mult)
{
	std::wstring str;

	for (int i = 32; i >= 2; i /= 2)
		if (mult == 1.0 / i)
		{
			str += L"1/" + std::to_wstring(i);
			return str;
		}

	int i = (int)mult;
	str += std::to_wstring(i) + L" beat";
	if (i > 1 || i == 0) str += L"s";

	return str;
}

double BPMRadialSlider::get_bpm_value()
{
	double val = 0;
	std::wstring bpm_str;

	switch (mode) {
	case BPMRadialSliderMode_Divisor:
		val = GetLFOSpeed(get_value());
		bpm_str = GetLFOSpeedString(val);
		break;
	case BPMRadialSliderMode_PerMinute:
		val = get_value() * MAX_BPM;
		bpm_str = std::to_wstring((int)val) + L" BPM";
		break;
	case BPMRadialSliderMode_BeatCount:
		val = (int)(get_value() * 64);
		bpm_str = std::to_wstring((int)val) + L" beat";
		if (val != 1)
			bpm_str += L"s";
	}

	if (bpm_str != content) {
		SetWContent(bpm_str);
	}

	return val;
}

