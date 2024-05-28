#pragma once
#include <d2d1.h>
#include <stdarg.h>

constexpr float PI = 3.141592;

#define DEBUG_PRINTF_MAX 512
#define MAX_BPM 250

inline constexpr double radians(double x) {
	return x * (PI / 180);
}

inline constexpr double map(double x, double fl, double fh, double tl, double th) {
	return (x-fl) * (th-tl) / (fh-fl) + tl;
}

inline constexpr double clamp(double x, double min, double max) {
	if (x > max) return max;
	if (x < min) return min;
	return x;
}

inline constexpr double mapc(double x, double fl, double fh, double tl, double th) {
	return map(clamp(x,fl,fh), fl, fh, tl, th);
}

inline float mod(float n, float d) {
	n = fmod(n, d);
	if (n < 0) n += d;
	return n;
}

inline ID2D1Factory* pFactory;

inline void debug_printf(const char* format, ...) {
//#ifdef NDEBUG
//	// DISABLE IN RELEASE BUILD
//	return;
//#endif

	va_list args;
	va_start(args, format);

	char* buf = new char[DEBUG_PRINTF_MAX];
	vsprintf_s(
		buf, DEBUG_PRINTF_MAX,
		format, args
	);

	OutputDebugStringA(buf);

	delete[] buf;
	va_end(args);
}
