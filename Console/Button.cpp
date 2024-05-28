#include "Button.h"

Button::Button() {
	control_name = "button";
	width = 250;
	height = 40;
	cornerRadius = 6;
	padding = 12;

	outline_color = foreground = 0x1A73C7;
	background = normal_background = 0x0f1113;
	hover_background = 0x15181b;

	this->font.FontName = L"Segoe UI";
	this->font.TextSize = 16;
	this->font.FontWeight = DWRITE_FONT_WEIGHT_NORMAL;

	this->use_hover_background_color = true;

	cursor = LoadCursor(NULL, IDC_HAND);

	this->Update();
	this->_UpdatePadding();
}

void Button::_UpdatePadding() {
	this->width = textMetrics.width + this->padding * 2;
	this->height = textMetrics.height + this->padding;
}

void Button::SetContent(std::string str, bool repaint)
{
	TextEnabledControl::SetContent(str, repaint);
	this->_UpdatePadding();
}

void Button::Paint(RenderTarget target)
{
	this->PaintBackground(target, true, true);
	this->PaintText(target);
}

void Button::OnClick() {
	if (this->callback != nullptr) {
		callback();
	}
}

void Button::SetClickEvent(std::function<void()> cb)
{
	this->callback = cb;
}
