#include "Console.h"

Console::Console(int width, int height)
{
    this->width = width;
    this->height = height;
    screenBuffer = new CHAR_INFO[width * height];
    bufferSize = { width, height };

    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    assert(outputHandle != INVALID_HANDLE_VALUE);
    assert(inputHandle != INVALID_HANDLE_VALUE);

    InitializeFont();
    InitializeSize(width, height);

    CONSOLE_CURSOR_INFO cci;

    cci.bVisible = false;
    cci.dwSize = 1;

    SetConsoleCursorInfo(outputHandle, &cci);
}

Console::~Console()
{
    if (screenBuffer != nullptr)
        delete[] screenBuffer;
}

void Console::Display()
{
    WriteConsoleOutput(outputHandle, screenBuffer, bufferSize, { 0, 0 }, &screenRect);
}

void Console::SetCursorPosition(int x, int y)
{
    SetConsoleCursorPosition(outputHandle, { x, y });
}

void Console::InitializeSize(int width, int height)
{
    screenRect.Top = 0;
    screenRect.Left = 0;
    screenRect.Right = width - 1;
    screenRect.Bottom = height - 1;

    SetConsoleScreenBufferSize(outputHandle, bufferSize);
    SetConsoleWindowInfo(outputHandle, TRUE, &screenRect);
}

void Console::InitializeFont()
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    GetCurrentConsoleFontEx(outputHandle, NULL, &cfi);

    cfi.FontWeight = 400;
    cfi.nFont = 2;
    cfi.dwFontSize = { 8, 8 };

    SetCurrentConsoleFontEx(outputHandle, NULL, &cfi);
}