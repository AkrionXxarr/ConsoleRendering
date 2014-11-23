#include <iostream>
#include <iomanip>
#include <sstream>

#include "GPU.h"
#include "Console.h"
#include "Object.h"
#include "RenderingEngine.h"
#include "PlaneComponent.h"
#include "Camera.h"
#include "Bitmap.h"
#include "FlatShader.h"
#include "TexturedShader.h"
#include "Time.h"

#define WIDTH 160
#define HEIGHT 120

const float zNear = 0.01f;

using namespace Math;

int main()
{ 
    ///////////////////////////////////
    // Initialize the GPU, renderer, camera, test shaders and test objects
    //

    Console console(WIDTH, HEIGHT);
    RenderingEngine renderingEngine;
    Object root;
    Time time;

    FlatShader fShader1('#', FOREGROUND_GREEN);
    FlatShader fShader2('@', FOREGROUND_RED);
    FlatShader fShader3('.', FOREGROUND_RED | FOREGROUND_GREEN);

    TexturedShader tShader1('#', "BrettPortrait.bmp");
    TexturedShader tShader2('@', "TestBitmap2.bmp");
	TexturedShader tShader3('#', "TestBitmap3.bmp");

    Object plane1;
    Object plane2;
    Object floor;

    Object camera;
    // Fov nearclip can't currently be anything but 0.01f 
    Camera camComponent(70.0f, float(WIDTH) / float(HEIGHT), zNear, 1000.0f);
    camera.AddComponent(&camComponent);
    camera.transform.LookAt(Vector3f(0, 0, 1), Vector3f(0, 1, 0));
    renderingEngine.camera = &camComponent;

    CHAR_INFO* screenBuffer = console.GetScreenBuffer();

    root.parent = nullptr;
    plane1.AddComponent(new PlaneComponent(&fShader2));
    plane2.AddComponent(new PlaneComponent(&tShader1));
    floor.AddComponent(new PlaneComponent(&tShader3));

    plane1.transform.pos = Vector3f(0, 0, 4);
    plane2.transform.pos = Vector3f(0, 0, 4);
    floor.transform.pos = Vector3f(0, -1, 4.25);

    plane2.transform.scale = Vector3f(3, 3, 3);
    floor.transform.scale = Vector3f(3, 3, 3);

    root.AddChild(&plane1);
    root.AddChild(&plane2);
    root.AddChild(&floor);

    float rad45 = float(M_PI / 180.0f) * -45;
    float rad90 = float(M_PI / 180.0f) * 90;
    floor.transform.Rotate(Vector3f(1, 0, 0), rad90);

    GPU::Get().SetConsoleHandle(&console);
    GPU::Get().ClearBuffers(true, true);


    //////////////////////////////
    // Define some variables specific to the loop
    //
    float c1 = 0, c2 = 0;
    float s1 = 0.1, s2 = 0.1;
    float camDist = 4.75f;
    float frameTimer = 0;
    float elapsedTime = 0;
    float runTime = 10; // Run for 10 seconds

    int frameCounter = 0;
    int lastFrameCount = 0;

    bool running = true;
    while (running)
    {
        time.Tick();
        frameTimer += time.deltaTime;
        elapsedTime += time.deltaTime;

        frameCounter++;
        if (frameTimer >= 1.0f)
        {
            lastFrameCount = frameCounter;
            frameCounter = 0;
            frameTimer = 0;
        }
        
        if (elapsedTime >= runTime)
        {
            running = false;
        }

        c1 += s1 * time.deltaTime;
        c2 += s2 * time.deltaTime;

        camera.transform.pos.z = (cos(c1) * camDist) + 4.25f;
        camera.transform.pos.x = sin(c1) * camDist;

        camera.transform.LookAt(plane2.transform.pos, Vector3f(0, 1, 0));
        
        plane1.transform.Rotate(Vector3f(1, 0, 0), -0.005f);
        plane1.transform.Rotate(Vector3f(0, 1, 0), 0.002f);
        plane1.transform.Rotate(Vector3f(0, 0, 1), 0.004f);

        plane2.transform.pos.y = sin(c2) * 1.5f;
        plane2.transform.pos.z = 4 + ((sin(c1) - 0.5f) * 5);
        plane2.transform.pos.x = cos(c1) * 1.25f;

        plane2.transform.pos.z = 4 + ((sin(c2) - 0.5f) * 5);

        plane2.transform.Rotate(Vector3f(1, 0, 0), -0.001f);
        plane2.transform.Rotate(Vector3f(0, 1, 0), -0.001f);
        plane2.transform.Rotate(Vector3f(0, 0, 1), 0.003f);

        renderingEngine.Render(&root);

        CHAR_INFO* sb = console.GetScreenBuffer();
        std::stringstream sStream;

        sStream << "elapsed time: " << std::setprecision(4) << elapsedTime;

        for (unsigned int i = 0; i < sStream.str().length(); i++)
        {
            CHAR_INFO* ci = &sb[i];
            ci->Attributes = BACKGROUND_INTENSITY | BACKGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
            ci->Char.UnicodeChar = sStream.str().c_str()[i];
        }

        sStream.str("");
        sStream << "fps: " << lastFrameCount;
        
        for (unsigned int i = 0; i < sStream.str().length(); i++)
        {
            CHAR_INFO* ci = &sb[WIDTH + i];
            ci->Attributes = BACKGROUND_INTENSITY | BACKGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
            ci->Char.UnicodeChar = sStream.str().c_str()[i];
        }


        console.Display();
    }

    root.Destroy();

    return 0;
}