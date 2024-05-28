#include "ListPanel.h"
#include "Window.h"

ListPanel::ListPanel()
{
	this->control_name = "list_panel";
	this->cornerRadius = 4;

	this->scrollbar.bypass_layout = true;

	// use base functions
	Control::AppendChild(&this->content);
	Control::AppendChild(&this->scrollbar);
}

void ListPanel::SetOrientation(Orientation orientation)
{
	content.SetOrientation(orientation);
}

void ListPanel::Paint(RenderTarget target)
{
	if (this->noScrollbar)
	{
		this->scrollbar.width = 0;
		this->scrollbar.isVisible = false;
	}

	this->scrollbar.height = this->height - (spacing * 2);
	this->width = content.width + this->scrollbar.width + (spacing * 2);

	this->scrollbar.location.x = width - spacing - scrollbar.width;
	this->scrollbar.location.y = spacing;

	this->content.location.x = 0;
	this->content.location.y = 0;

	//content.height = this->height;
	this->content.yOffset = -this->scrollbar.trackbarPos * max(content.height - height, 0);
	//this->content.yOffset = -15;

	graphics_utils::clip_control(target, this);

	this->PaintBackground(target, true, true);
	this->PaintChildren(target);

	target->PopAxisAlignedClip();
}

void ListPanel::OnChildrenModified()
{
	
}

void ListPanel::SetSpacing(float spacing)
{
	this->content.spacing = spacing;
	this->content.OnChildrenModified();

	this->spacing = spacing;
}

void ListPanel::SetContentWrapping(bool wrap)
{
	this->content.wrapContent = wrap;
}

void ListPanel::SetClientWidth(float width)
{
	this->content.width = width;
	this->content.OnChildrenModified();
}

void ListPanel::SetWidth(float width) {
	SetClientWidth(width - (this->scrollbar.width + (spacing * 2)));
}

void ListPanel::AppendChild(Control* control)
{
	this->content.AppendChild(control);
	//OutputDebugStringA(control->control_name);
}

void ListPanel::SetScrollbarEnabled(bool enabled)
{
	this->noScrollbar = !enabled;
}