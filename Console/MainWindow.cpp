#include "MainWindow.h"
#include "Oscillator.h"
#include "SoundMaker.h"
#include "CheckBox.h"
#include "Macro.h"
#include "AudioVisualizer.h"
#include "ProgressBar.h"

#ifndef PI
#define PI 3.14159
#endif
#include "WaveShapeRenderer.h"
#ifndef TWO_PI
#define TWO_PI (2.0 * PI)
#endif
//#include "FilterEffect.h"

MainWindow* instance;

std::vector<Oscillator*> oscillators;
std::vector<LFOGraphControl*> lfos;
std::vector<Macro*> macros;
std::vector<AudioEffect*> effects;



//std::atomic<double> tone = 261.63;
//SoundMaker<short>* sound;

double beatPos;

double lastBeatPos;
std::function<void()> onNextBeat;

double dTime;

//FilterEffect effectTest;

//STYPE* audio_buffer = nullptr;

void MainWindow::UpdateValues() {
	for (int i = 0; i < lfos.size(); i++) {
		lfos[i]->update(dTime,
			bpm_radial_slider.get_value());
	}
	for (int i = 0; i < macros.size(); i++) {
		macros[i]->update();
	}

	if (onNextBeat != nullptr) {
		beatPos = maths_utils::beats_to_lfo_pos(
			maths_utils::time_to_beats(dTime,
				bpm_radial_slider.value));

		if (beatPos < lastBeatPos) {
			onNextBeat();
			onNextBeat = nullptr;
		}
		lastBeatPos = beatPos;
	}

	soundMaker->recorder.record_beats =
		record_beats_length.get_bpm_value();
}



STYPE* MainWindow::GenerateSample(STYPE* buffer, int currentBlock, double dTimeStep, int m_nBlockSamples, int m_nChannels)
{
	instance->UpdateValues();

	for (int n = 0; n < m_nBlockSamples; n += m_nChannels)
	{
		for (Oscillator* osc : oscillators)
		{
			if (osc->IsEnabled())
			{
				instance->UpdateValues();

				double sample = osc->get_sample(dTime);
				//sample *= vol_radial_slider.get_value();
				sample = maths_utils::clip(sample, 1.0);

				for (int c = 0; c < m_nChannels; c++)
				{
					int index = currentBlock + n + c;
					buffer[index] += sample * SHRT_MAX;
				}
			}
		}

		dTime += dTimeStep;
	}

	for (AudioEffect* effect : effects)
	{
		effect->process(buffer, currentBlock, dTimeStep, m_nBlockSamples, m_nChannels);
	}

	//effectTest.process(buffer, currentBlock, dTimeStep, m_nBlockSamples, m_nChannels);

	return buffer;
}

void MainWindow::OnNextBeat(std::function<void()> cb)
{
	onNextBeat = cb;
}

STYPE* MainWindow::MakeNoise(STYPE* buffer, int currentBlock, double dTimeStep, int m_nBlockSamples, int m_nChannels)
{
	if (!enabledCheckBox.checked)
		return 0;


	// finally actually combine the audio of all oscillators
	return GenerateSample(
		buffer, currentBlock, dTimeStep,
		m_nBlockSamples, m_nChannels
	);
}

void MainWindow::CalculateLayout() {
	topControls.location = { 0,0 };
	topControls.SetWidth(width);
	topControls.height = 55;

	lfoListPanel.SetWidth(width / 2);
	oscListPanel.SetWidth(width / 2);

	oscListPanel.location = {
		0, topControls.height
	};

	lfoListPanel.location = {
		oscListPanel.location.x + oscListPanel.width,
		topControls.height
	};

	bottomControls.height = 150;
	bottomControls.location = {
		0, height - bottomControls.height
	};

	bottomControls.SetWidth(width);

	lfoListPanel.height = height / 1.75 - bottomControls.height;
	oscListPanel.height = lfoListPanel.height;


	macrosListPanel.location = {
		0, lfoListPanel.location.y + lfoListPanel.height
	};
	macrosListPanel.height = height - macrosListPanel.location.y - bottomControls.height;

	macrosListPanel.SetWidth(width / 2);

	effectsListPanel.SetWidth(width / 2);
	effectsListPanel.height = height - macrosListPanel.location.y - bottomControls.height;
	effectsListPanel.location = {
		oscListPanel.location.x + oscListPanel.width,
		lfoListPanel.location.y + lfoListPanel.height
	};


	effectsListPanel.isVisible =
		oscListPanel.isVisible =
		lfoListPanel.isVisible =
		macrosListPanel.isVisible = enabledCheckBox.checked;

	std::wstring str = std::to_wstring(
		(int)(bpm_radial_slider.get_value() * MAX_BPM)) + L" BPM";

	if (bpm_radial_slider.content != str) {
		bpm_radial_slider.SetWContent(str);
	}

	if (soundMaker != nullptr)
	{
		soundMaker->recorder.music_bpm =
			bpm_radial_slider.value * MAX_BPM;

		recordProgressBar.value = soundMaker->recorder.beats_recorded() /
			soundMaker->recorder.record_beats;
	}
}

void MainWindow::on_loaded()
{
	has_loaded = true;
}

void MainWindow::Paint(RenderTarget target)
{
	if (!has_loaded)
		return;

	this->CalculateLayout();
	Window::Paint(target);
}

void MainWindow::run()
{
	soundMaker = new SoundMaker(44100, 1);
	soundMaker->SetUserFunction(&MainWindow::MakeNoise);

	instance = this;

	this->animating = true;
	Window::run();

	soundMaker->Stop();
}

void MainWindow::AddNewOscillator() {
	auto osc = new Oscillator();
	osc->SetEnabled(true);
	oscListPanel.AppendChild(osc);
	oscillators.push_back(osc);
}

void MainWindow::AddNewLFO() {
	auto lfo = new LFOGraphControl();
	lfo->SetWContent(L"LFO " + std::to_wstring(lfos.size() + 1));
	lfo->SetEnabled(true);
	lfoListPanel.AppendChild(lfo);
	lfos.push_back(lfo);
}

void MainWindow::AddNewMacro() {
	auto macro = new Macro();
	macro->SetWContent(L"Macro " + std::to_wstring(macros.size() + 1));
	macro->SetEnabled(true);
	macrosListPanel.AppendChild(macro);
	macros.push_back(macro);
}

void MainWindow::AddNewEffect() {
	auto effect = new AudioEffect(
		soundMaker->m_nSampleRate,
		soundMaker->m_nChannels
	);
	effect->SetWContent(L"Effect " + std::to_wstring(effects.size() + 1));
	effectsListPanel.AppendChild(effect);
	effects.push_back(effect);
}

void NewOscillator() {
	instance->AddNewOscillator();
}

void NewLFO() {
	instance->AddNewLFO();
}

void NewMacro() {
	instance->AddNewMacro();
}

void NewEffect()
{
	instance->AddNewEffect();
}

void RecordOnNextBeat() {
	soundMaker->recorder.open_file("./output.wav");
}
void Record() {
	instance->OnNextBeat(&RecordOnNextBeat);
}

void MainWindow::setup()
{
	

	bpm_radial_slider.SetWContent(L"bpm");
	vol_radial_slider.SetWContent(L"mast");
	newOscillatorBtn.SetWContent(L"New Oscillator");
	newLFOBtn.SetWContent(L"New LFO");
	newMacroBtn.SetWContent(L"New Macro");
	newEffectBtn.SetWContent(L"New Effect");
	recordBtn.SetWContent(L"record (test)");

	newOscillatorBtn.SetClickEvent(&NewOscillator);
	newLFOBtn.SetClickEvent(&NewLFO);
	newMacroBtn.SetClickEvent(&NewMacro);
	recordBtn.SetClickEvent(&Record);
	newEffectBtn.SetClickEvent(&NewEffect);

	visualizer.width = 70;

	bottomControls.SetOrientation(HORIZONTAL);
	bottomControls.SetSpacing(10);
	bottomControls.SetScrollbarEnabled(false);
	bottomControls.SetContentWrapping(true);

	bottomControls.background = 0;
	bottomControls.outline_color = 0x1A73C7;
	
	bottomControls.CalculateLayout();

	newOscillatorBtn.width = 120;
	newLFOBtn.width = 120;
	newMacroBtn.width = 120;
	newEffectBtn.width = 120;
	recordBtn.width = 120;

	recordProgressBar.background = 0x15181b;
	recordProgressBar.outline_color = 0x1A73C7;

	record_panel.AppendChild(&record_beats_length);
	record_panel.AppendChild(&recordBtn);
	record_panel.AppendChild(&recordProgressBar);

	record_panel.SetSpacing(5);
	record_panel.height = 75;
	record_panel.background = 0;
	record_panel.outline_color = 0x1A73C7;
	record_panel.SetScrollbarEnabled(false);

	bottomControls.AppendChild(&visualizer);
	bottomControls.AppendChild(&bpm_radial_slider);
	bottomControls.AppendChild(&vol_radial_slider);
	bottomControls.AppendChild(&enabledCheckBox);
	
	bottomControls.AppendChild(&record_panel);

	topControls.AppendChild(&newOscillatorBtn);
	topControls.AppendChild(&newLFOBtn);
	topControls.AppendChild(&newMacroBtn);
	topControls.AppendChild(&newEffectBtn);

	topControls.SetScrollbarEnabled(false);
	topControls.SetContentWrapping(false);
	topControls.SetSpacing(10);
	topControls.background = 0;
	topControls.outline_color = 0x1A73C7;

	record_beats_length.SetMode(BPMRadialSliderMode_BeatCount);

	enabledCheckBox.checked = true;

	auto bc = 0;
	auto bo = 0x1A73C7;
	oscListPanel.background = bc;
	lfoListPanel.background = bc;
	macrosListPanel.background = bc;
	effectsListPanel.background = bc;

	oscListPanel.outline_color = bo;
	lfoListPanel.outline_color = bo;
	macrosListPanel.outline_color = bo;
	effectsListPanel.outline_color = bo;

	this->AppendChild(&topControls);
	this->AppendChild(&bottomControls);
	this->AppendChild(&oscListPanel);
	this->AppendChild(&macrosListPanel);
	this->AppendChild(&lfoListPanel);
	this->AppendChild(&effectsListPanel);

	lfoListPanel.SetSpacing(15);
	lfoListPanel.SetContentWrapping(true);
	lfoListPanel.SetOrientation(HORIZONTAL);

	oscListPanel.SetSpacing(15);
	oscListPanel.SetContentWrapping(true);
	oscListPanel.SetOrientation(HORIZONTAL);

	macrosListPanel.SetSpacing(15);
	macrosListPanel.SetContentWrapping(true);
	macrosListPanel.SetOrientation(HORIZONTAL);

	effectsListPanel.SetSpacing(15);
	effectsListPanel.SetContentWrapping(true);
	effectsListPanel.SetOrientation(HORIZONTAL);

	/*for (int i = 0; i < 1; i++) {
		AddNewOscillator();
	}
	for (int i = 0; i < 1; i++) {
		AddNewLFO();
	}
	for (int i = 0; i < 1; i++) {
		AddNewMacro();
	}*/
	
	vol_radial_slider.theta = .75f;
	bpm_radial_slider.theta = .5f;

	this->CalculateLayout();

	this->TryRepaintWindow();
	this->background = 0;
}

