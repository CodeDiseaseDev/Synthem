#pragma once
#include "TextEnabledControl.h"
#include "CheckBox.h"
#include "RadialSlider.h"
#include "DragNDropControl.h"

class Macro :
    public TextEnabledControl
{
private:
  CheckBox enabledCheckBox;
  RadialSlider valueRadialSlider;
  DragNDropControl assignDrag;

  double value;

public:
  Macro();

  bool IsEnabled();
  void SetEnabled(bool enabled);
  void update();

  virtual void Paint(RenderTarget target) override;
  virtual void CalculateLayout() override;
};

