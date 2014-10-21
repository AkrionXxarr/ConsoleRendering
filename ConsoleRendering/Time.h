#pragma once
#include <Windows.h>

class Time
{
public:
    static const int SECONDS = 1000;

    static float Get()
    {
        return float(GetTickCount()) / float(SECONDS);
    }
};