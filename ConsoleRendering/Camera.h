#pragma once

#include "Object.h"
#include "MathClasses.h"

class Camera : public Component
{
public:
    Camera(float fov, float aspect, float zNear, float zFar);

    void Destroy() { }

    void Update(float) { }
    void Render(RenderingEngine*) { }

    Math::Matrix4f GetViewProjection();

private:
    Math::Matrix4f projection;
};