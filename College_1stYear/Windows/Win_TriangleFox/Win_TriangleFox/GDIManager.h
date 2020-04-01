#pragma once

#include "CommonHeader.h"

class GDIManager
{
public:

	GDIManager();
	~GDIManager();

	void Initialize(HWND hWnd);
	void Release(HWND hWnd);

	void SetColor(const struct ColorRGB& InColor);
	void PutPixel(int InX, int InY);

private:

	ULONG CurrentColor;
	HDC HScreenDC;

};