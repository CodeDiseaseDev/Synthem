#pragma once
#include "Control.h"
#include "DragNDropControl.h"
#include "TextEnabledControl.h"
#include "CheckBox.h"
#include "BPMRadialSlider.h"

class LFOGraphControl :
    public TextEnabledControl
{
protected:

  // Must always be in correct (x lowest to heighest)
  // order to product correct graph
  std::vector<D2D1_POINT_2F> Points;

  DragNDropControl drag;
  CheckBox enabledCheckBox;
  BPMRadialSlider lfoBpm;

  double value;
  double xValue;

  int point_drag_index = -1;

public:
  LFOGraphControl() {
    this->control_name = "lfo_graph";
    this->width = 250;
    this->height = 150;

    this->cornerRadius = 4;

    outline_color = 0x1A73C7;
    background = 0x0f1113;

    AddPointInverseY(0, 0);
    AddPointInverseY(0.5, 0.25);
    AddPointInverseY(1, 0);

    this->font.TextSize = 12;
    this->font.FontWeight = DWRITE_FONT_WEIGHT_THIN;
    this->SetContent("LFO 1");

    drag.SetContent("*");
    drag.pTargetValue = &value;

    lfoBpm.background = { 0,0,0,0.75f };
    lfoBpm.theta = 0.75f;

    AppendChild(&drag);
    AppendChild(&enabledCheckBox);
    AppendChild(&lfoBpm);
  }

  virtual void Paint(RenderTarget target) override;
  virtual void OnDoubleClick(int x, int y) override;
  virtual void AddPoint(float x, float y);
  virtual void AddPointInverseY(float x, float y);
  virtual void mouse_move(int x, int y) override;
  virtual void mouse_down();
  virtual void mouse_up();
  virtual void mouse_drag(int x, int y);
  virtual void CalculateLayout() override;

  void PaintGrid(RenderTarget target);

  bool IsEnabled();
  void SetEnabled(bool enabled);

  int HitTestPoints();
  float GetMinXForPoint(int index);
  float GetMaxXForPoint(int index);

  void update(double dTime, double bpm);

  double InterpolateY(double x);
};

