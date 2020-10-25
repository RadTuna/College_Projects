#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <vector>

#include "Point.h"

class ConsoleRenderer final
{
public:
    explicit ConsoleRenderer(size_t width, size_t height);
    ~ConsoleRenderer();

    void DrawChar(const Point& point, int character, int order = 0);
    void DrawChar(int x, int y, int character, int order = 0);
    void DrawHorizontal(int y, int fromX, int toX, int character, int order = 0);
    void DrawVertical(int x, int fromY, int toY, int character, int order = 0);
    void SetColor(unsigned short color);

    void Present();

private:
    void Clear();
    void SetDepthBuffer(int x, int y, int order);
    int GetDepthBuffer(int x, int y);

private:
    HANDLE mFrontBuffer;
    HANDLE mBackBuffer;

    size_t mScreenWidth;
    size_t mScreenHeight;
    WORD mCurrentColor;

    std::vector<int> mDepthBuffer;

};
