#include "_PanelContent.h"
#include "ListPanel.h"


void _PanelContent::SetOrientation(
	Orientation orientation)
{
	this->orientation = orientation;
	this->OnChildrenModified();
	this->TryRepaintWindow();

	Window* wnd = FindParentWindow();
	if (wnd != nullptr) {
		wnd->_RetriggerMouseMove();
	}
}

void _PanelContent::Paint(RenderTarget target)
{
	graphics_utils::translate(target, 0, this->yOffset);

	this->PaintChildren(target);

	/*auto test = CreateSolidBrush(target, {0xff,0,0,0.4f});
	target->FillRectangle({ 0,0,width,height }, test);
	test->Release();*/

	graphics_utils::translate(target, 0, -this->yOffset);
}

void _PanelContent::OnChildrenModified()
{
	Control::OnChildrenModified();

	auto size = this->CalculateListPanelLayout(
		spacing, this->orientation, wrapContent);

	//OutputDebugStringA("hi\n");

	if (!wrapContent) {
		width = size.width;
		height = size.height;
	}
	else {
		switch (orientation) {
		case HORIZONTAL:
			height = size.height;
			break;
		case VERTICAL:
			width = size.width;
			break;
		}
	}
}
