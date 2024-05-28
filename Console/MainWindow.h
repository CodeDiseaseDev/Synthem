#pragma once
#include "Window.h"

#include "Button.h"
#include "RadialSlider.h"
#include "ListPanel.h"
#include "Slider.h"
#include "LFOGraphControl.h"
#include "AudioRecorder.h"
#include "AudioEffect.h"
#include "FilterEffect.h"
#include "ProgressBar.h"
#include "WaveShapeRenderer.h"
#include "AudioVisualizer.h"
#include "MainWindowControls.h"

inline class MainWindow : public Window
{
private:
	bool has_loaded;

	void UpdateValues();

	static STYPE* GenerateSample(STYPE* buffer, int currentBlock, double dTimeStep, int m_nBlockSamples, int m_nChannels);
	static STYPE* MakeNoise(STYPE* buffer, int currentBlock, double dTimeStep, int m_nBlockSamples, int m_nChannels);

	virtual void CalculateLayout() override;

	virtual void on_loaded() override;


public:
	void OnNextBeat(std::function<void()> cb);
	void Paint(RenderTarget target) override;
	void run() override;
	void AddNewOscillator();
	void AddNewLFO();
	void AddNewMacro();
	void AddNewEffect();
	void setup(); // called before run();




} win;
