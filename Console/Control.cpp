#include "Control.h"
#include "utils.h"
#include "Window.h"

void Control::PaintChildren(RenderTarget target, bool paintPost)
{
	for (auto child : children)
	{
		if (child == nullptr) {
			debug_printf("Control::PaintChildren failed because a child control was a null pointer!\n");
			return;
		}

		/*if (strcmp(child->control_name, "shape_renderer") == 0) {
			debug_printf("%s\n", child->control_name);
		}*/

		if (!child->isVisible)
			continue;

		

		graphics_utils::translate(
			target, child->location.x,
			child->location.y);

		D2D1_MATRIX_3X2_F transform;
		target->GetTransform(&transform);
		child->screen_location = { transform.dx, transform.dy };

		if (!paintPost)
			child->Paint(target);
		else
			child->PaintPost(target);

		// reverse the translation
		graphics_utils::translate(
			target, -child->location.x,
			-child->location.y);
	}
}

void Control::mouse_leave() {
	if (!use_hover_background_color)
		return;

	this->background = this->normal_background;
	this->RepaintWindow();
}

void Control::mouse_enter() {
	if (!use_hover_background_color)
		return;

	this->background = this->hover_background;
	this->RepaintWindow();
}

void Control::mouse_down() {

}

void Control::mouse_up() {

}

void Control::on_parented(Control* parent)
{
	debug_printf(
		"%s is parented by %s\n",
		this->control_name, parent->control_name
	);
}

D2D1_SIZE_F Control::CalculateListPanelLayout(float spacing, Orientation orientation, bool wrap) {
	float pos_x = spacing;
	float pos_y = spacing;
	float max_o = 0;
	float total_width = spacing;
	float total_height = spacing;
	bool new_row_or_column = false;

	for (auto child : children) {
		// skip
		if (child->bypass_layout) continue;

		float childPos_x = pos_x;
		float childPos_y = pos_y;

		switch (orientation) {
		case VERTICAL:
			if (wrap && pos_y + child->height > this->height) {
				pos_y = spacing;
				pos_x += max_o + spacing;
				max_o = 0;  // Reset max_o for the new column
				new_row_or_column = true;
			}

			childPos_x = pos_x;
			childPos_y = pos_y;

			child->location = { childPos_x, childPos_y };
			pos_y += child->height + spacing;

			max_o = max(child->width, max_o);
			break;

		case HORIZONTAL:
			if (wrap && pos_x + child->width > this->width) {
				pos_x = spacing;
				pos_y += max_o + spacing;
				max_o = 0;  // Reset max_o for the new row
				new_row_or_column = true;
			}

			childPos_x = pos_x;
			childPos_y = pos_y;

			child->location = { childPos_x, childPos_y };
			pos_x += child->width + spacing;

			max_o = max(child->height, max_o);
			break;
		}

		new_row_or_column = false;
	}

	if (wrap) {
		switch (orientation) {
		case HORIZONTAL:
			if (pos_x != spacing || new_row_or_column) {
				pos_y += max_o + spacing;
			}
			total_width = this->width;
			total_height = pos_y;
			break;
		case VERTICAL:
			if (pos_y != spacing || new_row_or_column) {
				pos_x += max_o + spacing;
			}
			total_width = pos_x;
			total_height = this->height;
			break;
		}
		return { total_width, total_height };
	}

	switch (orientation) {
	case HORIZONTAL:
		return { pos_x + spacing, max_o + spacing * 2 };
	case VERTICAL:
		return { max_o + spacing * 2, pos_y + spacing };
	}

	// Default return in case something goes wrong
	return { spacing, spacing };
}



//D2D1_SIZE_F Control::CalculateListPanelLayout(float spacing, Orientation orientation, bool wrap) {
//	float pos_x = spacing;
//	float pos_y = spacing;
//	float max_o = 0;
//
//	for (auto child : children) {
//		// skip
//		if (child->bypass_layout) continue;
//
//		float childPos_x = pos_x;
//		float childPos_y = pos_y;
//
//		switch (orientation) {
//		case VERTICAL:
//			child->location = { childPos_x, childPos_y };
//			pos_y += child->height + spacing;
//
//			if (wrap && pos_y > this->height - child->height) {
//				pos_y = spacing;
//				pos_x += max_o + spacing;
//			}
//
//			max_o = max(child->width, max_o);
//			break;
//
//		case HORIZONTAL:
//			child->location = { childPos_x, childPos_y };
//			pos_x += child->width + spacing;
//
//			if (wrap && pos_x > this->width - child->width) {
//				pos_x = spacing;
//				pos_y += max_o + spacing;
//			}
//
//			max_o = max(child->height, max_o);
//			
//			break;
//		}
//	}
//
//	if (wrap) {
//		switch (orientation)
//		{
//		case HORIZONTAL:
//			
//			if (pos_x != spacing)
//			{
//				pos_x = spacing;
//				pos_y += max_o + spacing;
//			}
//			break;
//		case VERTICAL:
//			
//			if (pos_y != spacing)
//			{
//				pos_y = spacing;
//				pos_x += max_o + spacing;
//			}
//			break;
//		}
//		return {
//			pos_x, pos_y
//		};
//	}
//
//	switch (orientation)
//	{
//	case HORIZONTAL:
//		return { pos_x + spacing * 2,max_o + spacing * 2 };
//	case VERTICAL:
//		return { max_o + spacing * 2,pos_y + spacing * 2 };
//	}
//}

void Control::PaintDisabledRectangle(RenderTarget target)
{
	auto b = this->CreateSolidBrush(target, ColorF{ 0,0,0,0.2f });
	target->FillRectangle({ 0,0,width,height }, b);
	b->Release();
}

void Control::mouse_drag(int x, int y)
{
}

void Control::mouse_wheel(int delta)
{

}

void Control::OnChildrenModified()
{
}

void Control::mouse_move(int x, int y)
{
	// handle mouse movements
}

void Control::OnDoubleClick(int x, int y)
{
}

void Control::AppendChild(Control* control)
{
	control->parent = this;
	printf("this->parent set in %s\n", control->control_name);
	this->children.push_back(control);

	this->OnChildrenModified();

	control->on_parented(this);
}

ColorF Control::LightenColor(ColorF color, float amount) {
	return {
		min(color.r + amount, 1),
		min(color.g + amount, 1),
		min(color.b + amount, 1)
	};
}

void Control::CalculateLayout() {

}

ID2D1SolidColorBrush* Control::CreateSolidBrush(RenderTarget target, ColorF color)
{
	ID2D1SolidColorBrush* pBrush;
	hr = target->CreateSolidColorBrush(color, &pBrush);

	if (FAILED(hr)) throw std::exception(
		"target->CreateSolidColorBrush failed");

	return pBrush;
}

void Control::PaintBackground(RenderTarget target, bool rounded, bool outline)
{
	ID2D1SolidColorBrush* pBrush;
	hr = target->CreateSolidColorBrush(background, &pBrush);

	ID2D1SolidColorBrush* pOutlineBrush = nullptr;
	if (outline)
		hr = target->CreateSolidColorBrush(
			outline_color, &pOutlineBrush);

	if (FAILED(hr)) throw std::exception(
		"target->CreateSolidColorBrush failed");

	auto rect = D2D1_RECT_F{ 0, 0, width, height };

	if (rounded)
	{
		D2D1_ROUNDED_RECT rRect{ rect, cornerRadius, cornerRadius };

		target->FillRoundedRectangle(
			rRect, pBrush);

		if (outline)
			target->DrawRoundedRectangle(
				rRect, pOutlineBrush);
	}
	else
	{
		target->FillRectangle(
			rect, pBrush);

		if (outline)
			target->FillRectangle(
				rect, pOutlineBrush);
	}

	pBrush->Release();
	if (pOutlineBrush != nullptr)
		pOutlineBrush->Release();
}

void Control::Paint(RenderTarget target)
{
	this->PaintChildren(target);
}

D2D1_POINT_2F Control::CenterScreenPoint()
{
	return {
		this->screen_location.x + width / 2,
		(this->screen_location.y - this->yOffset) + height / 2
	};
}

void Control::PaintPost(RenderTarget target)
{
	this->PaintChildren(target, true);
}


Control* Control::CursorHitTest(float x, float y, Control* ignore) {
	if (!Contains(x, y) || !isVisible) {
		return nullptr;
	}

	for (auto i = this->children.rbegin(); i != this->children.rend(); ++i) {
		Control* child = *i;

		// recursion
		Control* ctrl = child->CursorHitTest(
			x - this->location.x,
			y - this->location.y - yOffset,
			ignore
		);

		if (ctrl != nullptr && ignore != ctrl && ctrl->isVisible)
		{
			debug_printf("cursor hover control yOffset: %.2f\n", yOffset);
			return ctrl;
		}
	}

	return this;
}

void Control::OnClick() {

}

bool Control::Contains(float x, float y) {
	return (x >= this->location.x &&
		x <= this->location.x + this->width &&
		y >= this->location.y &&
		y <= this->location.y + this->height);
}

Window* Control::FindParentWindow()
{
	if (this->parent != nullptr) {
		return this->parent->FindParentWindow();
	}

	printf("this->parent null in %s\n", this->control_name);
	return nullptr;
}

void Control::RepaintWindow() {
	Control* wnd = (Control*)this->FindParentWindow();

	if (wnd == nullptr) {
		throw std::exception("this->FindParentWindow failed");
		return;
	}

	wnd->RepaintWindow();
}

bool Control::TryRepaintWindow() {
	Window* wnd = (Window*)this->FindParentWindow();

	if (wnd == nullptr ||
		wnd->pRenderTarget == nullptr) {
		return false;
	}

	wnd->RepaintWindow();
	return true;
}
