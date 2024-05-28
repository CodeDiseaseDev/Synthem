#include "AudioEffect.h"

AudioEffect::AudioEffect(int sampleRate, int channels)
{
	width = 250;
	height = 70;

	foreground = { 1,1,1,0.025 };
	background = 0x0f1113;
	outline_color = 0x1A73C7;
	cornerRadius = 6;

	filter = new FilterEffect(sampleRate, channels);

	AppendChild(&type_renderer);
	AppendChild(&enabledCheckBox);

	enabledCheckBox.checked = true;


	filterCutoff.SetWContent(L"Cutoff");
	filterQ.SetWContent(L"Reso");
	distortionDrive.SetWContent(L"Drive");
	gain.SetWContent(L"Gain");
	gain.set_value_range(0, 3, 1);

	AppendChild(&filterCutoff);
	AppendChild(&filterQ);

	AppendChild(&distortionDrive);

	AppendChild(&gain);

	effect_controls.push_back(&filterCutoff);
	effect_controls.push_back(&filterQ);

	effect_controls.push_back(&distortionDrive);

	effect_controls.push_back(&gain);

	type_renderer.SetOnChangeType([&]() {
		filter->SetFilterType(type_renderer.type);

		update_control_visibility();
	});

	type_renderer.SetType(LoPass);
	
}

void AudioEffect::update_control_visibility()
{
	for (Control* c : effect_controls)
		c->isVisible = false;

	switch (type_renderer.type)
	{
	case HiPass:
	case LoPass:
		// filters
		filterCutoff.isVisible = true;
		filterQ.isVisible = true;
		break;

	case SqDistortion:
		distortionDrive.isVisible = true;
		break;

	case Gain:
		gain.isVisible = true;
		break;
	}
}

void AudioEffect::process(STYPE* buf, int currentBlock, double dTimeStep, int m_nBlockSamples, int m_nChannels)
{
	if (!IsEnabled())
		return;

	switch (type_renderer.type)
	{
	case HiPass:
	case LoPass:
		filter->SetCutoff(
			filterCutoff.get_value_range(27, 8000),
			filterQ.get_value_range_squared(1,5)
		);

		filter->process(
			buf, currentBlock,
			dTimeStep, m_nBlockSamples,
			m_nChannels
		);
		break;

	case SqDistortion:
		for (int i = 0; i < m_nBlockSamples; i++)
		{
			for (int c = 0; c < m_nChannels; c++)
			{
				double sample = (double)buf[(currentBlock + i) + c] / SHRT_MAX;

				sample = Distort(sample, distortionDrive.get_value_range(1,50));

				buf[(currentBlock + i) + c] = (STYPE)(sample * SHRT_MAX);
			}

		}
		
		break;

	case Gain:
		for (int i = 0; i < m_nBlockSamples; i++)
		{
			for (int c = 0; c < m_nChannels; c++)
			{
				buf[(currentBlock + i) + c] *= gain.get_value_range(0, 3);
			}
		}
		break;
	}
}

void AudioEffect::CalculateLayout()
{
	type_renderer.location = { 10,10 };
	type_renderer.width = 75;
	type_renderer.height = height - type_renderer.location.y * 2;

	enabledCheckBox.location = { 3,3 };

	update_control_visibility();

	float space = 10;
	float x = type_renderer.location.x + type_renderer.width + space;
	for (Control* c : effect_controls)
	{
		if (!c->isVisible) continue;

		c->location = { x, height / 2 - c->height / 2 };
		x += c->width + space;
	}

}

void AudioEffect::Paint(RenderTarget target)
{
	CalculateLayout();

	PaintBackground(target, true, IsEnabled());
	PaintChildren(target);

	TextEnabledControl::Paint(target);

	if (!IsEnabled()) {
		PaintDisabledRectangle(target);
	}
}

bool AudioEffect::IsEnabled()
{
	return enabledCheckBox.checked;
}
