#pragma once
#include "Control.h"

class ProgressBar :
    public Control
{
public:
  double value = 0;

  ProgressBar() {
    width = 150;
    height = 25;

    cornerRadius = 4;
  }

  void Paint(RenderTarget target) override;

  void SetProgress(double value);
};

