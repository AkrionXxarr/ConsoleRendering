#pragma once
#include <Windows.h>

class Time
{
public:
    Time()
    {
        curTime = lastTime = GetTickCount() / 1000.0f;
        deltaTime = 0;
    }

    void Tick()
    {
        lastTime = curTime;
        curTime = GetTickCount() / 1000.0f;
        deltaTime = curTime - lastTime;
    }

public:
    float lastTime, curTime, deltaTime;
};