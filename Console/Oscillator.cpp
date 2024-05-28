#include "Oscillator.h"
#include "OpenFileDialog.h"
#include "Window.h"
#include "AudioEffect.h"

void Oscillator::Paint(RenderTarget target)
{
	//debug_printf("%.2f\n", this->oscFreq.get_value());

	CalculateLayout();

	PaintBackground(target, true, IsEnabled());
	PaintChildren(target);

	if (!IsEnabled()) {
		PaintDisabledRectangle(target);
	}
}

//inline double time_to_beats(double dTime) {
//	return (dTime * MAX_BPM) / 60.0f;
//}
//
//inline double beats_to_lfo_pos(double beats) {
//	// beats = 5.25
//	// beats - (int)beats = 5.25 - 5
//	// result = .25
//	return beats - (int)beats;
//}



inline const double TimeStep() {
	return 1.0 / soundMaker->m_nSampleRate;
}

std::vector<float> ResampleAudio(const std::vector<float>& inputBuffer, int inputSampleRate, int outputSampleRate) {
	// Calculate the resampling ratio
	double resampleRatio = static_cast<double>(outputSampleRate) / inputSampleRate;
	size_t inputSize = inputBuffer.size();
	size_t outputSize = static_cast<size_t>(inputSize * resampleRatio);

	std::vector<float> outputBuffer(outputSize);

	// Perform the resampling using linear interpolation
	for (size_t i = 0; i < outputSize; ++i) {
		double srcIndex = i / resampleRatio;
		size_t srcIndexFloor = static_cast<size_t>(std::floor(srcIndex));
		size_t srcIndexCeil = srcIndexFloor + 1;

		if (srcIndexCeil >= inputSize) {
			srcIndexCeil = inputSize - 1; // Clamp to the last element
		}

		double fraction = srcIndex - srcIndexFloor;

		// Linear interpolation
		outputBuffer[i] = inputBuffer[srcIndexFloor] * (1.0 - fraction) + inputBuffer[srcIndexCeil] * fraction;
	}

	return outputBuffer;
}

double Oscillator::get_sample(double dTime)
{
	if (!IsEnabled())
		return 0;

	double freq = GetFrequency();
	double lvl = this->oscLevel.get_value();
	double dist = this->oscDistortion.get_value();

	if (freq <= 0)
	{
		return 0;
	}

	/*OSC_SHAPE oscShape = (OSC_SHAPE)(shape * OSC_NOISE);
	shapeRenderer.oscillator_shape = oscShape;*/
	OSC_SHAPE oscShape = shapeRenderer.oscillator_shape;

	this->phase += TimeStep() * freq * PI * 2;

	const double osc_v = osc(phase, oscShape);
	//const double dis_v = Distort(osc_v, dist);

	return maths_utils::clip(osc_v * lvl, 1.0);
}

void Oscillator::AppendChild(Control*)
{
	// disable this
	return;
}

void Oscillator::CalculateLayout() {
	float spacing = 10;

	this->oscLevel.location = { spacing, spacing };
	this->oscFreq.location = {
		this->oscLevel.location.x + this->oscLevel.width + spacing,
		this->oscLevel.location.y
	};
	/*this->oscDistortion.location = {
		this->oscFreq.location.x + this->oscFreq.width + spacing,
		this->oscFreq.location.y
	};*/

	enabledCheckBox.location = {
		this->oscFreq.location.x + this->oscFreq.width + spacing,//this->oscDistortion.location.x + this->oscDistortion.width + spacing,
		height / 2 - enabledCheckBox.height / 2
	};

	shapeRenderer.location = {
		enabledCheckBox.location.x + enabledCheckBox.width + spacing,
		spacing
	};

	

	this->width = shapeRenderer.location.x + shapeRenderer.width + spacing;

	/*char str[12];
	sprintf_s(str, "Freq %.0fhz", GetFrequency());*/

	

}

bool Oscillator::IsEnabled()
{
	return enabledCheckBox.checked;
}

float Oscillator::GetFrequency()
{
	double freq = this->oscFreq.get_value();
	return freq * 5000;
}

void Oscillator::Setup()
{
	Control::AppendChild(&this->oscLevel);
	Control::AppendChild(&this->oscFreq);
	//Control::AppendChild(&this->oscDistortion);
	Control::AppendChild(&this->enabledCheckBox);
	Control::AppendChild(&this->shapeRenderer);

	shapeRenderer.SetOnClickCallback([&]()
	{
		if (shapeRenderer.oscillator_shape == OSC_SAMPLE)
		{
			std::wstring file;
			HWND hWindow = FindParentWindow()->Hwnd();

			if (SUCCEEDED(OpenFileDialog(hWindow, file)))
			{

			}
		}
	});

	this->oscLevel.SetContent("Level");
	this->oscFreq.SetContent("Frequency");
	this->oscDistortion.SetContent("Distortion");

	this->oscLevel.theta = 0.25f;
	this->oscFreq.theta = 0.05f;
	this->oscDistortion.theta = 0;

	/*oscLevel.background = 0;
	oscFreq.background = 0;
	oscShape.background = 0;
	oscDistortion.background = 0;*/

	this->CalculateLayout();
}

void Oscillator::SetEnabled(bool enabled) {
	enabledCheckBox.checked = enabled;
}
