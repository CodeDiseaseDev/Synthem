#include "TextEnabledControl.h"

void TextEnabledControl::Update() {
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(pDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory)
	);

	if (FAILED(hr))
		throw std::exception(
			"DWriteCreateFactory failed");

	hr = pDWriteFactory->CreateTextFormat(
		this->font.FontName, NULL,
		this->font.FontWeight,
		this->font.FontStyle,
		this->font.FontStretch,
		this->font.TextSize,
		L"", &pTextFormat
	);

	if (FAILED(hr))
		throw std::exception(
			"pDWriteFactory->CreateTextFormat failed");

	hr = pDWriteFactory->CreateTextLayout(
		content.c_str(),
		static_cast<UINT32>(content.length()),
		pTextFormat, FLT_MAX, FLT_MAX,
		&pTextLayout
	);

	pTextLayout->GetMetrics(&textMetrics);

	if (FAILED(hr))
		throw std::exception(
			"pDWriteFactory->CreateTextLayout failed");
}

void TextEnabledControl::SetContent(std::string str, bool repaint) {
	std::wstring wide(str.begin(), str.end());
	this->content = wide;

	this->Update();

	if (repaint) this->TryRepaintWindow();
}

void TextEnabledControl::SetWContent(std::wstring str, bool repaint)
{
	this->content = str;

	this->Update();

	if (repaint) this->TryRepaintWindow();
}

void TextEnabledControl::PaintText(RenderTarget target)
{
	

	hr = target->CreateSolidColorBrush(
		foreground, &pTextBrush
	);

	if (FAILED(hr))
		throw std::exception(
			"target->CreateSolidColorBrush failed");

	target->DrawTextLayout(
		{ 
			width / 2 - textMetrics.width / 2,
			height / 2 - textMetrics.height / 2,
		},
		pTextLayout,
		pTextBrush
	);

	pTextBrush->Release();
}

void TextEnabledControl::Paint(RenderTarget target)
{
	this->PaintText(target);
}
