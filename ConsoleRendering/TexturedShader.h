#pragma once

#include "Shader.h"
#include "Bitmap.h"

class TexturedShader : public Shader
{
public:
    TexturedShader(const char* fileName);
    ~TexturedShader();

    virtual void Update(Transform* transform, RenderingEngine* renderingEngine);

    Math::Vector4f VertexShader(Vertex* vertex);
    void FragmentShader(_CHAR_INFO* ci, Math::Vector2f* uv);

private:
    Bitmap bmp;
};