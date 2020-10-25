
#include "pch.h"
#include "ConsoleRenderer.h"

ConsoleRenderer::ConsoleRenderer(size_t width, size_t height)
    : mScreenWidth(width)
    , mScreenHeight(height)
{
    mFrontBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, 
        0, 
        nullptr, 
        CONSOLE_TEXTMODE_BUFFER, 
        nullptr);
    mBackBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, 
        0, 
        nullptr, 
        CONSOLE_TEXTMODE_BUFFER, 
        nullptr);

    CONSOLE_CURSOR_INFO consoleCursorInfo;
    consoleCursorInfo.bVisible = false;
    consoleCursorInfo.dwSize = 1;

    SetConsoleCursorInfo(mFrontBuffer, &consoleCursorInfo);
    SetConsoleCursorInfo(mBackBuffer, &consoleCursorInfo);

    COORD consoleScreenSize;
    consoleScreenSize.X = static_cast<SHORT>(width);
    consoleScreenSize.Y = static_cast<SHORT>(height);
    SetConsoleScreenBufferSize(mFrontBuffer, consoleScreenSize);
    SetConsoleScreenBufferSize(mBackBuffer, consoleScreenSize);

    mDepthBuffer.reserve(width * height);
    mDepthBuffer.assign(width * height, 0);
}

ConsoleRenderer::~ConsoleRenderer()
{
    CloseHandle(mFrontBuffer);
    CloseHandle(mBackBuffer);
}

void ConsoleRenderer::DrawChar(const Point& point, int character, int order)
{
    DrawChar(point.X, point.Y, character, order);
}

void ConsoleRenderer::DrawChar(int x, int y, int character, int order)
{
    if (GetDepthBuffer(x, y) > order)
    {
        return;
    }

    COORD cursorPosition;
    cursorPosition.X = x;
    cursorPosition.Y = y;

    SetConsoleCursorPosition(mBackBuffer, cursorPosition);
    WriteConsole(mBackBuffer, &character, 1, nullptr, nullptr);

    SetDepthBuffer(x, y, order);
}

void ConsoleRenderer::DrawHorizontal(int y, int fromX, int toX, int character, int order)
{
    assert(fromX <= toX);

    for (int x = fromX; x <= toX; ++x)
    {
        DrawChar(x, y, character, order);
    }
}

void ConsoleRenderer::DrawVertical(int x, int fromY, int toY, int character, int order)
{
    assert(fromY <= toY);

    for (int y = fromY; y <= toY; ++y)
    {
        DrawChar(x, y, character, order);
    }
}

void ConsoleRenderer::SetColor(unsigned short color)
{
    SetConsoleTextAttribute(mBackBuffer, color);
    SetConsoleTextAttribute(mFrontBuffer, color);
}

void ConsoleRenderer::Present()
{
    std::swap(mFrontBuffer, mBackBuffer);
    SetConsoleActiveScreenBuffer(mFrontBuffer);
    Clear();
}

void ConsoleRenderer::Clear()
{
    for (size_t i = 0; i < mDepthBuffer.size(); ++i)
    {
        mDepthBuffer[i] = -1;
    }

    for (int x = 0; x < mScreenWidth; ++x)
    {
        for (int y = 0; y < mScreenHeight; ++y)
        {
            DrawChar(x, y, ' ', -1);
        }
    }
}

void ConsoleRenderer::SetDepthBuffer(int x, int y, int order)
{
    mDepthBuffer[y * mScreenWidth + x] = order;
}

int ConsoleRenderer::GetDepthBuffer(int x, int y)
{
    return mDepthBuffer[y * mScreenWidth + x];
}
