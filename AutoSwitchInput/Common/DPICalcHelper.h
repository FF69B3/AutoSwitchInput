#pragma once
#include <windows.h>
#pragma comment(lib, "Gdi32.lib")

class DPICalcHelper
{
public:


	static int ConvertEpxToPixel(HWND hwnd, int effectivePixels)
	{
		float scalingFactor = GetScalingFactor(hwnd);
		return static_cast<int>(effectivePixels * scalingFactor);
	}

	static int ConvertPixelToEpx(HWND hwnd, int pixels)
	{
		float scalingFactor = GetScalingFactor(hwnd);
		return static_cast<int>(pixels / scalingFactor);
	}

	static float GetScalingFactor(HWND hwnd)
	{
		int dpi = GetDpiForWindow(hwnd);
		float scalingFactor = static_cast<float>(dpi) / 96;
		return scalingFactor;
	}

	static int GetDpiForWindow(HWND hwnd)
	{
		HDC hdc = GetDC(hwnd);
		int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
		ReleaseDC(hwnd, hdc);
		return dpi;
	}
};

