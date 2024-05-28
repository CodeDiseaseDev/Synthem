#pragma once
#include "Button.h"
#include "utils.h"

class DragNDropControl :
    public Button
{
  D2D1_POINT_2F drag_transform;
  std::vector<Control*> assignedControls;

  ColorF UniqueLinkColor = 0x0;

public:

  ColorF RandColor()
  {
    return {
      (rand() % 255) / 255.0f,
      (rand() % 255) / 255.0f,
      (rand() % 255) / 255.0f
    };
  }

  DragNDropControl() {
    cornerRadius = 6;

    UniqueLinkColor = this->LightenColor(RandColor(), 0.15);
    hover_background = UniqueLinkColor;
    background = UniqueLinkColor;

    this->use_hover_background_color = false;

    foreground = 0;

    this->font.FontName = L"Consolas";
    this->font.TextSize = 10;
  }

  Control* DragHitTest();

  void PaintPost(RenderTarget target) override;

  void DrawLineWithBorder(RenderTarget target, D2D1_POINT_2F a, D2D1_POINT_2F b, ID2D1Brush* brush, ID2D1Brush* outline);

  double* pTargetValue;

  void PaintMain(RenderTarget target);
  void Paint(RenderTarget target) override;
  void mouse_drag(int x, int y) override;
  void mouse_up() override;

  void PaintAssignedControlsLines(RenderTarget target, float opacity = .2f);
  
};

