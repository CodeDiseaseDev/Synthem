#pragma once
#include "Control.h"
#include <dwrite.h>
#include <string>
#include "Font.h"

class TextEnabledControl : public Control
{
protected:
	HRESULT hr;

	// used internally only \/
	IDWriteTextFormat* pTextFormat;
	IDWriteTextLayout* pTextLayout;
	IDWriteFactory* pDWriteFactory;

	DWRITE_TEXT_METRICS textMetrics;

	ID2D1SolidColorBrush* pTextBrush;

public:
	std::wstring content;
	ColorF foreground = ColorF::White;
	Font font;

	virtual void SetContent(std::string str, bool repaint = false);
	virtual void SetWContent(std::wstring str, bool repaint = false);
	void PaintText(RenderTarget target);
	virtual void Update();
	virtual void Paint(RenderTarget target) override;

	TextEnabledControl()
	{
		control_name = "text_enabled_ctrl_base";
	}
};

