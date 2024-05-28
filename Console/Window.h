#pragma once
#include <windows.h>
#include <chrono>
#include <windowsx.h>

#include "Control.h"
#include "global.h"
#include "Label.h"

using high_res_clock = std::chrono::high_resolution_clock;
using time_point = std::chrono::steady_clock::time_point;

template <class T> void SafeRelease(T** ppT)
{
  if (*ppT)
  {
    (*ppT)->Release();
    *ppT = NULL;
  }
}

class Window : public Control
{
protected:

  
  ID2D1SolidColorBrush* pBrush;

  Control* drag_control;


  D2D1_POINT_2F drag_last_point;

  D2D1_POINT_2F mouse_location;
  Control* cursor_hover_control;

  virtual void on_loaded();


  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  HRESULT CreateGraphicsResources();
  void    DiscardGraphicsResources();
  void    OnPaint();
  void    Resize();

  void _AnimationLoop();


public:
  ID2D1HwndRenderTarget* pRenderTarget;

  bool animating;

  void    StartAnimating();
  void    StopAnimating();

  bool Create(
    PCWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle = 0,
    int x = CW_USEDEFAULT,
    int y = CW_USEDEFAULT,
    int nWidth = 1300,
    int nHeight = 800,
    HWND hWndParent = 0,
    HMENU hMenu = 0
  );

  Window() : pRenderTarget(NULL), pBrush(NULL)
  {
    control_name = "window";
  }

  Window* FindParentWindow() override;
  void RepaintWindow() override;

  void Paint(RenderTarget target) override;

  HWND Hwnd() const { return m_hwnd; }

  virtual void    run();

  PCWSTR  ClassName() const { return L"CodeFormsD2D"; }
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

  void _PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

  void _RetriggerMouseMove();

protected:


  HWND m_hwnd;
};
