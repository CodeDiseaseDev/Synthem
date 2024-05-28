#pragma once
#include "Control.h"


class AudioVisualizer :
    public Control
{
public:

  AudioVisualizer();

  float equalizer_value;

  void Paint(RenderTarget target) override;
};

