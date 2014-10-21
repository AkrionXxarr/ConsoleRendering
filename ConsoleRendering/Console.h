#pragma once

#include <Windows.h>
#include <assert.h>

class Console
{
public:
    Console(int width, int height);
    ~Console();

    void Display();

    int GetBufferSize() { return bufferSize.X * bufferSize.Y; }
    COORD GetBufferSizeAsCoord() { return bufferSize; }
    CHAR_INFO* GetScreenBuffer() { return screenBuffer; }

    void SetCursorPosition(int x, int y);

private:
    void InitializeSize(int width, int height);
    void InitializeFont();

private:
    int width, height;
    SMALL_RECT screenRect;
    HANDLE outputHandle;
    HANDLE inputHandle;

    CHAR_INFO* screenBuffer;
    COORD bufferSize;
};