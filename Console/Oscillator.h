#pragma once
#include "Control.h"
#include "RadialSlider.h"
#include "CheckBox.h"
#include "SoundMaker.h"
#include "Oscillators.h"
#include "WaveShapeRenderer.h"

class Oscillator :
    public Control
{
	bool has_setup;

	double phase;
	double local_dTime;

	STYPE* audio_buffer;

public:

	RadialSlider oscLevel = RadialSlider();
	RadialSlider oscFreq = RadialSlider();
	RadialSlider oscDistortion = RadialSlider();
	WaveShapeRenderer shapeRenderer;

	CheckBox enabledCheckBox;

  void Paint(RenderTarget target) override;
  double get_sample(double dTime);

	//void IntoSampleBuffer(STYPE* buffer, int currentBlock, double dTimeStep, int m_nBlockSamples, int m_nChannels);


	void AppendChild(Control*) override;

	virtual void CalculateLayout() override;

	bool IsEnabled();

	float GetFrequency();


	Oscillator() {
		width = 250;
		height = 65;

		cornerRadius = 4;
		outline_color = 0x1A73C7;
		background = 0x0f1113;

		Setup();

		
	}

	void Setup();

	void SetEnabled(bool enabled);

	

	
};

