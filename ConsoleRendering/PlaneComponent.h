#pragma once
#include "Object.h"

class Mesh;
class RenderingEngine;
class FlatShader;

class PlaneComponent : public Component
{
public:
    PlaneComponent(char c, unsigned short attribute);
    ~PlaneComponent();

    virtual void Update(float delta);
    virtual void Render(RenderingEngine* renderingEngine);

private:
    Mesh* mesh;
    FlatShader* shader;
};