#include <iostream>
#include "GPU.h"
#include "Console.h"
#include "Object.h"
#include "RenderingEngine.h"
#include "PlaneComponent.h"
#include "Camera.h"
#include "Bitmap.h"
#include "FlatShader.h"
#include "TexturedShader.h"

#include "TextLogger.h"

#define WIDTH 160
#define HEIGHT 120

using namespace Math;

int main()
{
    Console console(WIDTH, HEIGHT);
    RenderingEngine renderingEngine;
    Object root;
    Bitmap bmp;

    bmp.Load24("TestBitmap.bmp");
    //return 0;

    FlatShader fShader1('#', FOREGROUND_GREEN);
    FlatShader fShader2('@', FOREGROUND_RED);
    FlatShader fShader3('.', FOREGROUND_RED | FOREGROUND_GREEN);

    TexturedShader tShader1('#', "TestBitmap.bmp");
    TexturedShader tShader2('@', "TestBitmap2.bmp");
    TexturedShader tShader3('.', "TestBitmap3.bmp");

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
    plane1.AddComponent(new PlaneComponent(&tShader1));
    plane2.AddComponent(new PlaneComponent(&tShader2));
    floor.AddComponent(new PlaneComponent(&tShader3));

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
        camera.transform.pos.z = (cos(counter) * camDist) + 4.25f;
        camera.transform.pos.x = sin(counter) * camDist;
        //camera.transform.pos.y = 2;

        //camera.transform.pos.x = constant + sin(angle) * scalar;
        //camera.transform.pos.z = -constant + cos(angle) * scalar;
        //angle = angle + speed;
        
        //camera.transform.LookAt(Vector3f(camera.transform.pos.x, -1, camera.transform.pos.z), Vector3f(0, 0, 1));
        camera.transform.LookAt(plane1.transform.pos, Vector3f(0, 1, 0));
        
        plane1.transform.Rotate(Vector3f(1, 0, 0), -0.005f);
        plane1.transform.Rotate(Vector3f(0, 1, 0), 0.002f);
        plane1.transform.Rotate(Vector3f(0, 0, 1), 0.004f);
        //plane1.transform.LookAt(plane2.transform.pos, Vector3f(0, 1, 0));

        plane2.transform.pos.y = sin(counter2) * 1.5f;
        plane2.transform.pos.z = 4 + ((sin(counter) - 0.5f) * 5);
        plane2.transform.pos.x = cos(counter) * 1.25f;

        plane2.transform.Rotate(Vector3f(1, 0, 0), -0.001f);
        plane2.transform.Rotate(Vector3f(0, 1, 0), -0.001f);
        plane2.transform.Rotate(Vector3f(0, 0, 1), 0.003f);

        root.UpdateAll(0.01f);
        renderingEngine.Render(&root);
        console.Display();
        counter += 0.002f;
        counter2 += 0.0075f;
    }

    return 0;
}