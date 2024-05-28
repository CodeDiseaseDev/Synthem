#include "Window.h"

HRESULT Window::CreateGraphicsResources()
{
  HRESULT hr = S_OK;
  if (pRenderTarget == NULL)
  {
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    hr = pFactory->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(m_hwnd, size),
      &pRenderTarget);

    if (SUCCEEDED(hr))
    {
      const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
      hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
    }
  }
  return hr;
}

void Window::DiscardGraphicsResources()
{
  SafeRelease(&pRenderTarget);
  SafeRelease(&pBrush);
}

void Window::OnPaint()
{
  /*if (showDebugFps)
  {
    fpsCount++;
    auto ns = high_res_clock::now() - lastCheck;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(ns);
    if (ms.count() > msBetweenChecks)
    {
      fps = fpsCount / ((float)ms.count() / 1000);

      lastCheck = high_res_clock::now();
      fpsCount = 0;
    }

    std::string fpsLabelStr = std::to_string(fps) + " FPS";
    fpsCountLabel.SetContent(fpsLabelStr);
  }*/

  HRESULT hr;

  PAINTSTRUCT ps;
  BeginPaint(Hwnd(), &ps);

  pRenderTarget->BeginDraw();

  // looks the best in this config :>
  pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
  pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

  this->Paint(pRenderTarget);
  this->PaintPost(pRenderTarget);

  hr = pRenderTarget->EndDraw();
  EndPaint(Hwnd(), &ps);

  if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
  {
    DiscardGraphicsResources();
  }
}

void Window::Resize()
{
  RECT rc;
  GetClientRect(m_hwnd, &rc);

  D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

  width = size.width;
  height = size.height;

  if (pRenderTarget != NULL)
  {
    pRenderTarget->Resize(size);
    //debug_printf("h\n");
  }

  TryRepaintWindow();
}

void Window::_PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  PostMessage(this->Hwnd(), uMsg, wParam, lParam);
}

void Window::StartAnimating()
{
  //this->showDebugFps = false;

  animating = true;
  //this->RepaintWindow();
}

void Window::StopAnimating()
{
  //this->showDebugFps = false;

  animating = false;
  //RepaintWindow();
}

Window* Window::FindParentWindow()
{
  return this;
}

void Window::_AnimationLoop() {
  MSG msg = {};
  while (animating)
  {
    //printf("window is animating\n");
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT) exit(0);

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      OnPaint();
    }
  }
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
void Window::run()
{
  if (!Create(L"Synthem", WS_OVERLAPPEDWINDOW))
  {
    throw std::exception("failed to create window");
  }

  ShowWindow(this->Hwnd(), SW_SHOWNORMAL);
  HRESULT hr = CreateGraphicsResources();

  Resize();

  if (FAILED(hr))
  {
    throw std::exception("CreateGraphicsResources failed");
    return;
  }

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    if (msg.message == WM_QUIT) exit(0);

    TranslateMessage(&msg);
    DispatchMessage(&msg);

    if (animating)
      _AnimationLoop();
  }
}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
    if (FAILED(D2D1CreateFactory(
      D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
    {
      return -1;  // Fail CreateWindowEx.
    }


    this->on_loaded();
    
    return 0;

  case WM_DESTROY:
    DiscardGraphicsResources();
    SafeRelease(&pFactory);
    PostQuitMessage(0);
    return 0;

  case WM_PAINT:
  {
    OnPaint();
  }

    //this->_RetriggerMouseMove();
    return 0;

    // Other messages not shown...

  case WM_SIZE:
    Resize();
    return 0;

  case WM_MOUSEMOVE:
    {
      int xPos = GET_X_LPARAM(lParam);
      int yPos = GET_Y_LPARAM(lParam);

      this->mouse_location = { (float)xPos, (float)yPos };

      if (drag_control != nullptr) {
        //printf("%.2f, %.2f\n", mouse_location.x, mouse_location.y);
        drag_control->mouse_drag(
          drag_last_point.x - mouse_location.x,
          drag_last_point.y - mouse_location.y
        );
        drag_last_point = mouse_location;
        return 0;
      }

      Control* ctrl = this->CursorHitTest(xPos, yPos);

      if (ctrl != nullptr)
      {
        ctrl->mouse_hover_location = {
          mouse_location.x - ctrl->screen_location.x,
          mouse_location.y - ctrl->screen_location.y
        };
        ctrl->mouse_move(
          ctrl->mouse_hover_location.x,
          ctrl->mouse_hover_location.y
        );
      }

      if (cursor_hover_control == ctrl)
        return 0;

      if (ctrl == nullptr) {
        cursor_hover_control = nullptr;
        return 0;
      }

      

      

      SetCursor(ctrl->cursor);

      if (cursor_hover_control != nullptr)
      {
        cursor_hover_control->isCursorHovering = false;
        cursor_hover_control->mouse_leave();

        debug_printf("mouse leave %s\n", cursor_hover_control->control_name);
      }

      ctrl->isCursorHovering = true;
      ctrl->mouse_enter();

      cursor_hover_control = ctrl;

      RepaintWindow();
    }
    return 0;

  case WM_LBUTTONDBLCLK:
    if (cursor_hover_control != nullptr) {
      cursor_hover_control->OnDoubleClick(
        mouse_location.x - cursor_hover_control->screen_location.x,
        mouse_location.y - cursor_hover_control->screen_location.y
      );
    }
    break;

  case WM_MOUSEWHEEL:
    if (cursor_hover_control != nullptr) {
      cursor_hover_control->mouse_wheel(
        GET_WHEEL_DELTA_WPARAM(wParam));
    }
    break;

  case WM_LBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_RBUTTONDOWN:
    SetCapture(Hwnd());
    if (cursor_hover_control != nullptr) {
      cursor_hover_control->OnClick();
      cursor_hover_control->mouse_down();
      drag_control = cursor_hover_control;
      drag_last_point = mouse_location;
    }
    return 0;

  case WM_LBUTTONUP:
  case WM_MBUTTONUP:
  case WM_RBUTTONUP:
    ReleaseCapture();
    if (cursor_hover_control != nullptr) {
      cursor_hover_control->mouse_up();
    }
    if (drag_control != nullptr)
      drag_control = nullptr;
    return 0;

  case WM_EXITSIZEMOVE:
    RepaintWindow();
    return 0;
  }

  return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void Window::_RetriggerMouseMove()
{
  LPARAM lParam = MAKELPARAM(
    mouse_location.x, mouse_location.y
  );

  this->_PostMessage(
    WM_MOUSEMOVE, 0, lParam
  );
}

void Window::Paint(RenderTarget target)
{
  this->PaintBackground(target, false);
  this->PaintChildren(target);

  //if (showDebugFps)
  //  fpsCountLabel.Paint(target);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  Window* pThis = NULL;

  if (uMsg == WM_NCCREATE)
  {
    CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
    pThis = (Window*)pCreate->lpCreateParams;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

    pThis->m_hwnd = hwnd;
  }
  else
  {
    pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  }
  if (pThis)
  {
    return pThis->HandleMessage(uMsg, wParam, lParam);
  }
  else
  {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
}

bool Window::Create(
  PCWSTR lpWindowName,
  DWORD dwStyle, DWORD dwExStyle,
  int x, int y, int nWidth, int nHeight,
  HWND hWndParent, HMENU hMenu
)
{
  this->width = nWidth;
  this->height = nHeight;

  WNDCLASS wc = { 0 };

  srand(time(NULL));

  wc.lpfnWndProc = Window::WindowProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = ClassName();
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.style = CS_DBLCLKS;
  //wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  RegisterClass(&wc);

  m_hwnd = CreateWindowEx(
    dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
    nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
  );

  return (m_hwnd ? TRUE : FALSE);
}

void Window::on_loaded()
{
  
}

void Window::RepaintWindow() {
  if (animating)
    return;

  OnPaint();
}
