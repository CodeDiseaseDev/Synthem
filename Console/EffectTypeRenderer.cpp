#include "EffectTypeRenderer.h"
#include "AudioEffect.h"

EffectTypeRenderer::EffectTypeRenderer()
{
  background = 0x1D2126;
  cornerRadius = 6;
}

void EffectTypeRenderer::Paint(RenderTarget target)
{
  PaintBackground(target, true, false);
	
  switch (type)
  {
  case LoPass:
  case HiPass:
    RenderFilterGraph(target);
    break;

  }

}

void EffectTypeRenderer::OnClick()
{
  int next_type = (int)type + 1;
  if (next_type >= AUDIO_EFFECT_TYPE_ITEMS)
  {
    next_type = 0;
  }

  this->type = (AudioEffectType)next_type;

  if (_onChangeType != nullptr)
    _onChangeType();
}

void EffectTypeRenderer::RenderFilterGraph(RenderTarget target)
{
  // Create a solid brush.
  auto brush = this->CreateSolidBrush(target, 0x6F63B1);

  float spacing = 5;
  float curve = 0.75;
  float hardness = 1.2;

  D2D1_POINT_2F controlPoint;
  D2D1_POINT_2F endPoint;
  D2D1_POINT_2F beginPoint;

  switch (type)
  {
  case LoPass:
    controlPoint = D2D1::Point2F(width * curve * hardness, height * (1 - curve));
    beginPoint = D2D1::Point2F(spacing, spacing);
    endPoint = D2D1::Point2F(width - spacing, height - spacing);
    break;

  case HiPass:
    controlPoint = D2D1::Point2F(width * (1 - curve) * hardness, height * (1 - curve));
    beginPoint = D2D1::Point2F(width - spacing, spacing);
    endPoint = D2D1::Point2F(spacing, height - spacing);
    break;

  default:
    controlPoint = D2D1::Point2F(width * (1 - curve) * hardness, height * (1 - curve));
    beginPoint = D2D1::Point2F(spacing, height / 2 + spacing);
    endPoint = D2D1::Point2F(width - spacing, height / 2 - spacing);
    break;
  }

  // Create a path geometry.
  ID2D1PathGeometry* pPathGeometry = nullptr;
  ID2D1Factory* fact = nullptr;
  target->GetFactory(&fact);

  HRESULT hr = fact->CreatePathGeometry(&pPathGeometry);
  if (SUCCEEDED(hr))
  {
    // Create a geometry sink to define the curve.
    ID2D1GeometrySink* pSink = nullptr;
    hr = pPathGeometry->Open(&pSink);
    if (SUCCEEDED(hr))
    {
      // Begin drawing the curve.
      pSink->BeginFigure(beginPoint, D2D1_FIGURE_BEGIN_FILLED);

      // Add a quadratic bezier segment to represent the low-pass filter curve.
      pSink->AddQuadraticBezier(
        D2D1::QuadraticBezierSegment(
          controlPoint,   // Control point
          endPoint    // End point
        )
      );

      // End drawing the curve.
      pSink->EndFigure(D2D1_FIGURE_END_OPEN);

      // Close the geometry sink.
      hr = pSink->Close();
      pSink->Release();
    }

    // Draw the path geometry to the render target.
    target->DrawGeometry(pPathGeometry, brush);

    pPathGeometry->Release();
  }

  // Release the brush.
  brush->Release();
}

void EffectTypeRenderer::SetOnChangeType(std::function<void()> func)
{
  this->_onChangeType = func;
}

void EffectTypeRenderer::SetType(AudioEffectType type)
{
  this->type = type;
}
