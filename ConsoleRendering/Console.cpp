#include "Console.h"

Console::Console(int width, int height)
{
    this->width = width;
    this->height = height;
    this->screenBuffer = new CHAR_INFO[width * height];
    this->bufferSize = { width, height };

    this->outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    this->inputHandle = GetStdHandle(STD_INPUT_HANDLE);

    assert(this->outputHandle != INVALID_HANDLE_VALUE);
    assert(this->inputHandle != INVALID_HANDLE_VALUE);

    this->InitializeFont();
    this->InitializeSize(width, height);

    CONSOLE_CURSOR_INFO cci;

    cci.bVisible = false;
    cci.dwSize = 1;

    SetConsoleCursorInfo(this->outputHandle, &cci);
}

Console::~Console()
{
    delete[] this->screenBuffer;
}

void Console::Display()
{
    WriteConsoleOutput(outputHandle, screenBuffer, bufferSize, { 0, 0 }, &screenRect);
}

void Console::SetCursorPosition(int x, int y)
{
    SetConsoleCursorPosition(this->outputHandle, { x, y });
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

    GetCurrentConsoleFontEx(this->outputHandle, NULL, &cfi);

    cfi.FontWeight = 400;
    cfi.nFont = 2;
    cfi.dwFontSize = { 8, 8 };

    SetCurrentConsoleFontEx(this->outputHandle, NULL, &cfi);
}