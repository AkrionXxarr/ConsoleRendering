#include <Windows.h>

#include "GPU.h"
#include "Mesh.h"
#include "TexturedShader.h"
#include "Transform.h"
#include "RenderingEngine.h"
#include "MathClasses.h"
#include "Camera.h"

using namespace Math;

TexturedShader::TexturedShader(char c, const char* fileName)
{
    this->c = c;
    this->bmp.Load24(fileName);
}

TexturedShader::~TexturedShader()
{
}

void TexturedShader::Update(Transform* transform, RenderingEngine* renderingEngine)
{
    worldMatrix = transform->GetTransformation();
    MVPMatrix = renderingEngine->camera->GetViewProjection() * worldMatrix;
}

Vector4f TexturedShader::VertexShader(Vertex* vertex)
{
    return MVPMatrix.Transform(Vector4f(vertex->pos, 1));
}

void TexturedShader::FragmentShader(_CHAR_INFO* ci, Math::Vector2f* uv)
{
    unsigned short attribute = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE 
                             | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE;

    if (bmp.loaded)
    {
        attribute = 0;
        int x = int((uv->x * (bmp.width / 2)) + (bmp.width / 2));
        int y = int(-(uv->y * (bmp.height / 2)) + (bmp.height / 2));

        RGB rgb = bmp.GetPixel(x, y);

        if (rgb.r == 128)
        {
            attribute |= FOREGROUND_RED;
        }
        else if (rgb.r == 255)
        {
            attribute |= FOREGROUND_INTENSITY | FOREGROUND_RED;
        }

        if (rgb.g == 128)
        {
            attribute |= FOREGROUND_GREEN;
        }
        else if (rgb.g == 255)
        {
            attribute |= FOREGROUND_INTENSITY | FOREGROUND_GREEN;
        }

        if (rgb.b == 128)
        {
            attribute |= FOREGROUND_BLUE;
        }
        else if (rgb.b == 255)
        {
            attribute |= FOREGROUND_INTENSITY | FOREGROUND_BLUE;
        }
    }

    ci->Attributes = attribute;
    ci->Char.UnicodeChar = c;
}