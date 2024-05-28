#include "DragNDropControl.h"
#include "Window.h"

void DragNDropControl::PaintMain(RenderTarget target)
{
	Button::Paint(target);
}

Control* DragNDropControl::DragHitTest()
{
	return FindParentWindow()->CursorHitTest(
		this->screen_location.x + drag_transform.x + width / 2,
		this->screen_location.y + drag_transform.y + height / 2,
		this
	);
}

void DragNDropControl::PaintPost(RenderTarget target) {
	PaintAssignedControlsLines(
		target,
		isCursorHovering ? 1 : 0.1f
	);
	/*if (isCursorHovering) {
		
	}*/

	if (drag_transform.x != 0 &&
		drag_transform.y != 0)
	{
		graphics_utils::translate(
			target, drag_transform.x,
			drag_transform.y);

		PaintMain(target);

		graphics_utils::translate(
			target, -drag_transform.x,
			-drag_transform.y);
	}
}

void DragNDropControl::DrawLineWithBorder(RenderTarget target, D2D1_POINT_2F a, D2D1_POINT_2F b, ID2D1Brush* brush, ID2D1Brush* outline)
{

	target->DrawLine(
		a, b,
		outline, 6
	);

	target->DrawLine(
		a, b,
		brush, 2
	);

	
}

void DragNDropControl::PaintAssignedControlsLines(RenderTarget target, float opacity)
{
	auto lc = UniqueLinkColor;
	auto lo = ColorF{ 0 };

	lc.a = opacity;
	lo.a = opacity;



	auto brush = this->CreateSolidBrush(target, lc);
	auto brushOutline = this->CreateSolidBrush(target, lo);

	for (auto ctrl : assignedControls)
	{
		ID2D1DrawingStateBlock* pDrawingStateBlock = nullptr;
		pFactory->CreateDrawingStateBlock(&pDrawingStateBlock);

		if (pDrawingStateBlock == nullptr)
			continue;

		target->SaveDrawingState(pDrawingStateBlock);

		target->SetTransform(D2D1::Matrix3x2F::Identity());
		auto p1 = ctrl->CenterScreenPoint();
		auto p2 = this->CenterScreenPoint();
		auto p3 = D2D1_POINT_2F{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
		p3 = D2D1_POINT_2F{ (p3.x + p1.x) / 2, (p3.y + p2.y) / 2 };
		//float lineWidth = 2;

		DrawLineWithBorder(
			target, p1, p3, // { p1.x, p2.y }
			brush, brushOutline
		);
		DrawLineWithBorder(
			target, p3, p2,
			brush, brushOutline
		);

		target->RestoreDrawingState(pDrawingStateBlock);
		pDrawingStateBlock->Release();
	}

	

	brush->Release();
	brushOutline->Release();
}

void DragNDropControl::Paint(RenderTarget target)
{


	PaintMain(target);

	//PaintAssignedControlsLines(target);

	
}

void DragNDropControl::mouse_drag(int x, int y)
{
	drag_transform.x += -x;
	drag_transform.y += -y;
}

void DragNDropControl::mouse_up()
{
	Control* ctrl = DragHitTest();

	if (ctrl != nullptr && ctrl->canHoldValue)
	{
		if (std::find(ctrl->valueSources.begin(), ctrl->valueSources.end(), pTargetValue) ==
			ctrl->valueSources.end())
		{
			ctrl->valueSources.push_back(pTargetValue);
			debug_printf("assigned lfo to %s\n", ctrl->control_name);

			assignedControls.push_back(ctrl);

			std::wstring wstr = L"";
			wstr += std::to_wstring(assignedControls.size());
			//wstr += L")";
			SetWContent(wstr);
		}
	}

	drag_transform = { 0, 0 };
}
