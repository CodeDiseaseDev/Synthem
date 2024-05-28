#pragma once
#include "Control.h"
#include <WinUser.h>
#include "ScrollBar.h"
#include "utils.h"
#include "_PanelContent.h"
#include "Orientation.h"

class ListPanel : public Control
{
protected:
  ScrollBar scrollbar;
  _PanelContent content;

  float spacing;

  bool noScrollbar = false;

  float test = 0;

public:
  ListPanel();

  Orientation orientation = HORIZONTAL;

  void SetOrientation(Orientation orientation);
  virtual void Paint(RenderTarget target) override;
  virtual void OnChildrenModified() override;
  void SetSpacing(float spacing);
  void SetContentWrapping(bool wrap);
  void SetClientWidth(float width);
  void SetWidth(float width);
  void SetScrollbarEnabled(bool enabled);

  virtual void AppendChild(Control* control) override;
};

