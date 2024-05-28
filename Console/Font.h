#pragma once
#include <dwrite.h>

/*
hr = pDWriteFactory->CreateTextFormat(
	L"Yu Gothic UI", NULL,
	DWRITE_FONT_WEIGHT_BOLD,
	DWRITE_FONT_STYLE_NORMAL,
	DWRITE_FONT_STRETCH_NORMAL,
	18, L"", &pTextFormat
);*/

struct Font {
	const wchar_t* FontName = L"Yu Gothic UI";
	DWRITE_FONT_WEIGHT FontWeight = DWRITE_FONT_WEIGHT_BOLD;
	DWRITE_FONT_STYLE FontStyle = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH FontStretch = DWRITE_FONT_STRETCH_NORMAL;
	float TextSize = 18;

};