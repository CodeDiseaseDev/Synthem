#include "ScrollBar.h"

void ScrollBar::Paint(RenderTarget target)
{
  this->cornerRadius = is_horizontal() ? this->height / 2 : this->width / 2;
  this->PaintBackground(target, true, true);

  auto brush = this->CreateSolidBrush(target, 0x1A73C7);

  float t_width = is_horizontal() ? this->width * trackbarSize : this->width - (trackbarMargin * 2);
  float t_height = is_horizontal() ? this->height - (trackbarMargin * 2) : this->height * trackbarSize;
  float t_x = is_horizontal() ? (this->width - t_width - (trackbarMargin * 2)) * trackbarPos + trackbarMargin : trackbarMargin;
  float t_y = is_horizontal() ? trackbarMargin : (this->height - t_height - (trackbarMargin * 2)) * trackbarPos + trackbarMargin;

  auto rect = D2D1_RECT_F{
      t_x, t_y,
      t_width + t_x,
      t_height + t_y
  };

  float rrCornerRadius = is_horizontal() ? t_height / 2 : t_width / 2;
  D2D1_ROUNDED_RECT rRect{ rect, rrCornerRadius, rrCornerRadius };

  target->FillRoundedRectangle(rRect, brush);

  brush->Release();
}

void ScrollBar::OffsetPos(float offset) {
  // clamp to 0 to 1
  trackbarPos += offset;
  trackbarPos = max(0, min(1, trackbarPos));

  this->RepaintWindow();
}

void ScrollBar::mouse_drag(int x, int y) {
  float offset = 0;
  if (is_horizontal()) {
    offset = -x / (this->width - (trackbarMargin * 2));
  }
  else {
    offset = -y / (this->height - (trackbarMargin * 2));
  }

  OffsetPos(offset);
}

void ScrollBar::mouse_wheel(int delta)
{
  OffsetPos((-delta / 120.0f) / 25);
}

void ScrollBar::SetOrientation(Orientation orientation)
{
  this->orientation = orientation;

  float _height = height;
  height = width;
  width = _height;
}
