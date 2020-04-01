#include "GDIManager.h"
#include "TMath.h"

GDIManager::GDIManager()
{

}

GDIManager::~GDIManager()
{

}

void GDIManager::Initialize(HWND hWnd)
{
	HScreenDC = GetDC(hWnd);
}

void GDIManager::Release(HWND hWnd)
{
	ReleaseDC(hWnd, HScreenDC);
}

void GDIManager::SetColor(const ColorRGB& InColor)
{
	CurrentColor = RGB(InColor.Red, InColor.Green, InColor.Blue);
}

void GDIManager::PutPixel(int InX, int InY)
{
	SetPixel(HScreenDC, InX, InY, CurrentColor);
}