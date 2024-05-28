#pragma once
#include "Control.h"
#include "Window.h"

class _PanelContent : public Control
{
public:

  _PanelContent() {
    this->control_name = "panel_content";
  }

  Orientation orientation = HORIZONTAL;
  float spacing = 5;
  bool wrapContent = false;

  void SetOrientation(Orientation orientation);
  virtual void Paint(RenderTarget target) override;
  virtual void OnChildrenModified() override;
};

