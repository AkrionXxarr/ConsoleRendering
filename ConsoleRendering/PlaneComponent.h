#pragma once
#include "Object.h"

class Mesh;
class RenderingEngine;
class Shader;

class PlaneComponent : public Component
{
public:
    PlaneComponent(Shader* shader);
    ~PlaneComponent();

    virtual void Update(float delta);
    virtual void Render(RenderingEngine* renderingEngine);

private:
    Mesh* mesh;
    Shader* shader;
};