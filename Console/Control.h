#pragma once

#include <d2d1.h>
#include <vector>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include "global.h"
#include "Orientation.h"

using namespace D2D1;
typedef ID2D1HwndRenderTarget* RenderTarget;

class Window;

class Control
{
private:
	HRESULT hr;
	
public:
	ColorF hover_background = ColorF(0x282828);
	ColorF normal_background = ColorF(0x181818);
	ColorF background = ColorF(0x181818);
	ColorF foreground = ColorF(0x181818);
	ColorF outline_color = ColorF(0xffffff);

protected:
	Control* parent; // dont use this
	//ColorF* render_background = &background;
	float duration = 5;


public:
	void on_parented(Control* ctrl);
	const char* control_name = "NO NAME!";

	D2D1_POINT_2F mouse_hover_location;
	D2D1_POINT_2F location;
	D2D1_POINT_2F screen_location = { -1, -1 };

	// used only by controls that have a value
	std::vector<double*> valueSources;
	bool canHoldValue;

	float width = 500,
				height = 250;

	bool bypass_layout = false;

	bool isVisible = true;
	bool isCursorHovering = false;
	int cornerRadius = 12;

	float yOffset;


	bool use_hover_background_color = false;

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);

	// please use Control::AppendChild to add child control.
	std::vector<Control*> children;

	void mouse_leave();
	void mouse_enter();

	virtual void mouse_drag(int x, int y);
	virtual void mouse_wheel(int delta);

	bool Contains(float x, float y);
	Control* CursorHitTest(float x, float y, Control* ignore = nullptr);
	ColorF LightenColor(ColorF color, float amount);
	
	virtual void CalculateLayout();

	ID2D1SolidColorBrush* CreateSolidBrush(RenderTarget target, ColorF color);

	void PaintBackground(RenderTarget target, bool rounded = false, bool outline = false);
	void PaintChildren(RenderTarget target, bool paintPost = false);
	void PaintDisabledRectangle(RenderTarget target);

	D2D1_SIZE_F CalculateListPanelLayout(float spacing, Orientation orientation, bool wrap);

	virtual void AppendChild(Control* control);

	virtual void Paint(RenderTarget target);
	virtual void PaintPost(RenderTarget target);
	virtual Window* FindParentWindow();

	D2D1_POINT_2F CenterScreenPoint();

	virtual void RepaintWindow();
	virtual bool TryRepaintWindow();

	virtual void mouse_move(int x, int y);
	virtual void mouse_down();
	virtual void mouse_up();

	virtual void OnChildrenModified();

	virtual void OnClick();
	virtual void OnDoubleClick(int x, int y);
};

