#include <iostream>
#include "GPU.h"
#include "Console.h"
#include "Object.h"
#include "RenderingEngine.h"
#include "PlaneComponent.h"
#include "Camera.h"

#include "TextLogger.h"

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
    Object floor;

    Object camera;
    Camera camComponent(70.0f, float(WIDTH) / float(HEIGHT), 0.01f, 1000.0f);
    camera.AddComponent(&camComponent);
    camera.transform.LookAt(Vector3f(0, 0, 1), Vector3f(0, 1, 0));
    renderingEngine.camera = &camComponent;


    CHAR_INFO* screenBuffer = console.GetScreenBuffer();

    root.parent = nullptr;
    plane1.AddComponent(new PlaneComponent('#', FOREGROUND_INTENSITY | FOREGROUND_GREEN));
    plane2.AddComponent(new PlaneComponent('@', FOREGROUND_RED));
    floor.AddComponent(new PlaneComponent('.', FOREGROUND_RED | FOREGROUND_GREEN));

    plane1.transform.pos = Vector3f(0, 0, 4.5);
    plane2.transform.pos = Vector3f(0, 0, 4);
    floor.transform.pos = Vector3f(0, -1, 4.25);

    floor.transform.scale = Vector3f(3, 3, 3);

    root.AddChild(&plane1);
    root.AddChild(&plane2);
    root.AddChild(&floor);

    float rad45 = float(M_PI / 180.0f) * -45;
    float rad90 = float(M_PI / 180.0f) * 90;

    floor.transform.Rotate(Vector3f(1, 0, 0), rad90);

    //plane1.transform.Rotate(Vector3f(0, 0, 1), -rad45);
    //plane1.transform.Rotate(Vector3f(0, 1, 0), rad45);


    GPU::Get().SetConsoleHandle(&console);

    for (int i = 0; i < console.GetBufferSize(); i++)
    {
        screenBuffer[i].Attributes = 0;
        screenBuffer[i].Char.UnicodeChar = ' ';
    }

    //camera.transform.pos.y = 4;
    float counter = 0;
    float counter2 = 0;
    float camDist = 4.75f;
    while (true)
    {
        camera.transform.pos.z = (cos(counter) * camDist) + 4.25;
        //camera.transform.pos.z = 1.0f;
        camera.transform.pos.x = sin(counter) * camDist;
        //camera.transform.pos.y = 2;

        //camera.transform.pos.x = constant + sin(angle) * scalar;
        //camera.transform.pos.z = -constant + cos(angle) * scalar;
        //angle = angle + speed;
        
        //camera.transform.LookAt(Vector3f(camera.transform.pos.x, -1, camera.transform.pos.z), Vector3f(0, 0, 1));
        camera.transform.LookAt(plane2.transform.pos, camera.transform.rot.GetUp());
        
        plane1.transform.Rotate(Vector3f(1, 0, 0), -0.05f);
        plane1.transform.Rotate(Vector3f(0, 1, 0), 0.02f);
        plane1.transform.Rotate(Vector3f(0, 0, 1), 0.04f);

        plane2.transform.pos.y = sin(counter2) * 1.5;
        plane2.transform.pos.z = 4 + ((sin(counter) - 0.5f) * 5);
        plane2.transform.pos.x = cos(counter) * 1.25;

        plane2.transform.Rotate(Vector3f(1, 0, 0), -0.01f);
        plane2.transform.Rotate(Vector3f(0, 1, 0), -0.01f);
        plane2.transform.Rotate(Vector3f(0, 0, 1), 0.03f);

        root.UpdateAll(0.01f);
        renderingEngine.Render(&root);
        console.Display();
        counter += 0.02f;
        counter2 += 0.075f;
    }

    return 0;
}