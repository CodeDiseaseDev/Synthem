#pragma once
#include <functional>
#include "TextEnabledControl.h"
class Button :
    public TextEnabledControl
{
protected:
  std::function<void()> callback;

  void _UpdatePadding();

public:
  Button();

  float padding;

  void SetContent(std::string str, bool repaint = false) override;

  virtual void Paint(RenderTarget target);
  virtual void OnClick() override;

  void SetClickEvent(std::function<void()> cb);
};

