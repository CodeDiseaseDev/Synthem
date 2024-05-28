#pragma once
#include "ListPanel.h"
#include "RadialSlider.h"
#include "CheckBox.h"
#include "Button.h"
#include "BPMRadialSlider.h"
#include "ProgressBar.h"
#include "AudioVisualizer.h"
#include "WaveShapeRenderer.h"

inline ListPanel oscListPanel;
inline ListPanel lfoListPanel;
inline ListPanel macrosListPanel;
inline ListPanel effectsListPanel;

inline RadialSlider bpm_radial_slider;
inline RadialSlider vol_radial_slider;
inline CheckBox enabledCheckBox;
inline Button newOscillatorBtn;
inline Button newLFOBtn;
inline Button newMacroBtn;
inline Button newEffectBtn;
inline AudioVisualizer visualizer;
inline WaveShapeRenderer shapeRenderer;
inline ListPanel bottomControls;

inline ListPanel topControls;


inline ListPanel record_panel;
inline BPMRadialSlider record_beats_length;
inline ProgressBar recordProgressBar;
inline Button recordBtn;