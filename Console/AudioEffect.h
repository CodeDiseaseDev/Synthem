#pragma once
#include "SoundMaker.h"
#include "Control.h"
#include "TextEnabledControl.h"
#include "EffectTypeRenderer.h"
#include "CheckBox.h"
#include "FilterEffect.h"
#include "RadialSlider.h"

constexpr auto _____AUDIO_EFFECT_TYPE_LINE = __LINE__;
enum AudioEffectType {
	LoPass,
	HiPass,
	SqDistortion, 
	Gain
};
constexpr auto AUDIO_EFFECT_TYPE_ITEMS = __LINE__ - _____AUDIO_EFFECT_TYPE_LINE - 3;

inline double Distort(double sample, double distortion) {
	/*distortion *= distortion;
	distortion *= 2;
	distortion += 1;

	sample *= distortion;
	sample = maths_utils::clamp(sample, -1, 1);

	return sample;*/

	/*sample = sample + distortion * sample * sample * sample;
	sample = maths_utils::clamp(sample, -1, 1);*/

	sample = tanh(distortion * sample);

	//sample = mod(distortion * sample + 1, 2) - 1;

	//sample = abs(mod(2 * distortion * sample + 2, 4) - 2) - 1;

	//sample = sin(distortion * sample);

	return sample;
}

class AudioEffect :
	public TextEnabledControl
{
	CheckBox enabledCheckBox;

	FilterEffect* filter;

	std::vector<Control*> effect_controls;

	RadialSlider filterCutoff;
	RadialSlider filterQ;

	RadialSlider distortionDrive;

	RadialSlider gain;

public:
	AudioEffect(int sampleRate, int channels);

	void update_control_visibility();

	EffectTypeRenderer type_renderer;

	virtual void process(
		STYPE* buf, int currentBlock,
		double dTimeStep, int m_nBlockSamples,
		int m_nChannels
	);

	void CalculateLayout();

	void Paint(RenderTarget target) override;

	bool IsEnabled();
};

