#include "Label.h"

void Label::Update() {
	TextEnabledControl::Update();

	width = textMetrics.width;
	height = textMetrics.height;
}
