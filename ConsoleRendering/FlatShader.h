#pragma once

#include "Shader.h"

// Ignores UVs and such, just generates a single color with a single character
class FlatShader : public Shader
{
public:
    FlatShader(char c, unsigned short attribute);
    ~FlatShader();

    virtual void Update(Transform* transform, RenderingEngine* renderingEngine);

    void VertexShader(Vertex* vertex);
    void FragmentShader(_CHAR_INFO* ci);

private:
    char c;
    unsigned short attribute;
};