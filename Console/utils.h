#pragma once
#include <d2d1.h>
#include "Control.h"

namespace graphics_utils {
	inline void translate(ID2D1HwndRenderTarget* target, float x, float y) {
		D2D1_MATRIX_3X2_F transform;
		target->GetTransform(&transform);
		transform.dx += x;
		transform.dy += y;
		target->SetTransform(transform);
	}

	inline void translate_ctrl_pre(ID2D1HwndRenderTarget* target, Control* ctrl) {
		translate(target, ctrl->location.x, ctrl->location.y);
	}

	inline void translate_ctrl_post(ID2D1HwndRenderTarget* target, Control* ctrl) {
		translate(target, -ctrl->location.x, -ctrl->location.y);
	}

	inline void clip_control(ID2D1HwndRenderTarget* target, Control* ctrl) {
		target->PushAxisAlignedClip(
			D2D1::RectF(0,0,ctrl->width,ctrl->height),
			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
		);
	}
	
	
}

namespace maths_utils {
	inline double clamp(double x, double min, double max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	inline double time_to_beats(double dTime, double bpm) {
		return (dTime * bpm * MAX_BPM) / 60.0f;
	}

	template <typename T>
	inline T max_value() {
		return pow(2, sizeof(T) * 8 - 1) - 1;
	}

	inline double beats_to_lfo_pos(double beats) {
		// beats = 5.25
		// beats - (int)beats = 5.25 - 5
		// result = .25
		return beats - (int)beats;
	}

	template<class T>
	inline T lerp_t(T start, T end, T t) {
		return (1.0 - t) * start + t * end;
	}

	inline double clip(double dSample, double dMax)
	{
		if (dSample >= 0.0)
			return fmin(dSample, dMax);
		else
			return fmax(dSample, -dMax);
	}

	inline double lerp(double start, double end, double t) {
		return lerp_t(start, end, t);
	}
}