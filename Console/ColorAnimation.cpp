#include "ColorAnimation.h"

void ColorAnimation::Update(float deltaTime)
{
  if (IsFinished())
    return;

  elapsedTime = min(elapsedTime + deltaTime, *duration);

  float t = elapsedTime / *duration;

  float r = easingFunction(t, startColor->r, endColor->r - startColor->r, 1.0f);
  float g = easingFunction(t, startColor->g, endColor->g - startColor->g, 1.0f);
  float b = easingFunction(t, startColor->b, endColor->b - startColor->b, 1.0f);

  *target = ColorF(r, g, b);

  if (IsFinished() && callback)
    callback();
}

bool ColorAnimation::IsFinished() const
{
  return elapsedTime >= *duration;
}

void ColorAnimation::SetCallback(std::function<void()> callbackFunction)
{
  callback = std::move(callbackFunction);
}

float ColorAnimation::EaseLinear(float t, float b, float c, float d)
{
	return c * t / d + b;
}

float ColorAnimation::EaseInOutQuad(float t, float b, float c, float d)
{
	t /= d / 2;
	if (t < 1) return c / 2 * t * t + b;
	t--;
	return -c / 2 * (t * (t - 2) - 1) + b;
}

