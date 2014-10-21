#pragma once

#include "MathClasses.h"

struct _CHAR_INFO;
struct Vertex;
class Transform;
class RenderingEngine;

class Shader
{
public:
    Shader() { }
    virtual ~Shader() { }

    void Bind();
    virtual void Update(Transform* transform, RenderingEngine* renderingEngine) = 0;

    virtual void VertexShader(Vertex* vertex) = 0;
    virtual void FragmentShader(_CHAR_INFO* ci) = 0;

public:
    Math::Matrix4f worldMatrix;
    Math::Matrix4f MVPMatrix;
};