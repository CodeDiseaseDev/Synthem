#pragma once
#include "Control.h"
#include "TextEnabledControl.h"
#include "utils.h"

class RadialSlider;

class RadialSlider :
    public TextEnabledControl
{
public:
  RadialSlider* inner;

  double theta = 0;

  double value;
  double value_eased;

  double get_value();
  double get_value_range(double min, double max);
  void set_value_range(double min, double max, double value);
  double get_value_range_squared(double min, double max);

  ColorF color = 0x1A73C7;

  RadialSlider(bool containsInner = true);

  D2D1_POINT_2F _InterpolateTheta(float theta, float r);

  virtual void Paint(RenderTarget target) override;
  virtual void CalculateLayout() override;
  void mouse_drag(int x, int y) override;
};

