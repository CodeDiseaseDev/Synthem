#pragma once
#include "Control.h"
class CheckBox :
    public Control
{
public:
  bool checked = false;

  CheckBox();

  virtual void CalculateLayout() override;

  virtual void Paint(RenderTarget target) override;
  virtual void OnClick() override;
};

