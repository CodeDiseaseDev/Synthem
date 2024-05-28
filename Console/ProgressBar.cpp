#include "ProgressBar.h"

void ProgressBar::Paint(RenderTarget target)
{
	this->PaintBackground(target, true, true);

	//auto brush1 = CreateSolidBrush(target, background);
	auto brush2 = CreateSolidBrush(target, foreground);

	//target->FillRoundedRectangle(D2D1_ROUNDED_RECT{ { 0,0,width,height }, 4,4 }, brush1);
	target->FillRoundedRectangle(D2D1_ROUNDED_RECT{ { 0,0,(float)value * width,height}, (float)cornerRadius, (float)cornerRadius}, brush2);

	brush2->Release();
	//brush2->Release();
}

void ProgressBar::SetProgress(double value)
{
	this->value = value;
}
