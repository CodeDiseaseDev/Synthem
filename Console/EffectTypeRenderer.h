#pragma once
#include "Control.h"
#include <functional>

enum AudioEffectType;

class EffectTypeRenderer :
    public Control
{
private:
	std::function<void()> _onChangeType;

public:
	EffectTypeRenderer();

	AudioEffectType type;

	void Paint(RenderTarget target) override;
	void OnClick() override;

	void RenderFilterGraph(RenderTarget target);

	void SetOnChangeType(std::function<void()> func);
	void SetType(AudioEffectType type);
};

