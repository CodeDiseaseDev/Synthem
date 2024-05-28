#pragma once
#include <cmath>
#include <algorithm>
#include <functional>

#include "Window.h"

using EasingFunction = float(*)(float, float, float, float);

class ColorAnimation {
public:
  ColorAnimation(ColorF* target, ColorF* start, ColorF* end, float* duration, EasingFunction easing, Control* wnd)
    : target(target), startColor(start), endColor(end),
      duration(duration), elapsedTime(0.0f), easingFunction(easing),
      TargetWindow((Window*)wnd) {};

  void Update(float deltaTime);
  bool IsFinished() const;

  void SetCallback(std::function<void()> callbackFunction);

  static float EaseLinear(float t, float b, float c, float d);
  static float EaseInOutQuad(float t, float b, float c, float d);

protected:
  ColorF* target;
  ColorF* startColor;
  ColorF* endColor;
  float* duration;
  float elapsedTime;

  Window* TargetWindow;

  EasingFunction easingFunction;

private:
  std::function<void()> callback;
};

