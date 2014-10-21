#pragma once

class Object;
class Camera;

class RenderingEngine
{
public:
    RenderingEngine() { }
    ~RenderingEngine() { }

    void Render(Object* obj);

public:
    Camera* camera;
};