#pragma once
#include "Control.h"

class Slider :
    public Control
{
public:
  float position = 0.5f;

  Slider() {
    this->height = 10;
    foreground = 0x333333;
    background = 0x111111;
  }

  virtual void Paint(RenderTarget target) override;

  void OffsetPos(float offset);
  void mouse_drag(int x, int y) override;
};

