#pragma once
#include "Control.h"
#include "TextEnabledControl.h"

// Label can literally just be an alias of TextEnabledControl...
class Label : public TextEnabledControl {
public:
	void Update() override;
};
