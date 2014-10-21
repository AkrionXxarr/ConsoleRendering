#include <iostream>
#include "GPU.h"
#include "Console.h"
#include "Object.h"
#include "RenderingEngine.h"
#include "PlaneComponent.h"
#include "Camera.h"

#define WIDTH 160
#define HEIGHT 120

using namespace Math;

int main()
{
    Console console(WIDTH, HEIGHT);
    RenderingEngine renderingEngine;
    Object root;

    Object plane1;
    Object plane2;

    Object camera;
    Camera camComponent(70.0f, WIDTH / HEIGHT, 0.01f, 1000.0f);
    camera.AddComponent(&camComponent);
    camera.transform.LookAt(Vector3f(0, 0, 1), Vector3f(0, 1, 0));
    renderingEngine.camera = &camComponent;


    CHAR_INFO* screenBuffer = console.GetScreenBuffer();

    root.parent = nullptr;
    plane1.AddComponent(new PlaneComponent('#', FOREGROUND_INTENSITY | FOREGROUND_GREEN));
    plane2.AddComponent(new PlaneComponent('@', FOREGROUND_RED));

    plane1.transform.scale = Vector3f(0.5f, 0.5f, 1);
    plane2.transform.scale = Vector3f(0.5f, 0.5f, 1);

    plane1.transform.pos = Vector3f(0.025, 0, 0.25);
    plane2.transform.pos = Vector3f(-0.025f, 0, 0);

    root.AddChild(&plane1);
    root.AddChild(&plane2);

    GPU::Get().SetConsoleHandle(&console);

    for (int i = 0; i < console.GetBufferSize(); i++)
    {
        screenBuffer[i].Attributes = 0;
        screenBuffer[i].Char.UnicodeChar = ' ';
    }

    float counter = 0;
    while (true)
    {
        plane1.transform.Rotate(Vector3f(1, 0, 0), 0.02f);
        plane1.transform.Rotate(Vector3f(0, 1, 0), 0.02f);
        plane1.transform.Rotate(Vector3f(0, 0, 1), 0.02f);

        plane2.transform.Rotate(Vector3f(1, 0, 0), -0.02f);
        plane2.transform.Rotate(Vector3f(0, 1, 0), -0.02f);
        plane2.transform.Rotate(Vector3f(0, 0, 1), -0.02f);

        root.UpdateAll(0.01f);
        renderingEngine.Render(&root);
        console.Display();
        counter += 0.1f;
    }

    // O 0 1 I i L l
    return 0;
}