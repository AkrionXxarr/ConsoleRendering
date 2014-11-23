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
    bmp.Load24(fileName);
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

		if (x < 0) { x = 0; }
		if (y < 0) { y = 0; }
		if (x >= bmp.width) { x = bmp.width - 1; }
		if (y >= bmp.height) { y = bmp.height - 1; }

        RGB rgb = bmp.GetPixel(x, y);
        //RGB rgb = bmp.GetPixel(uv->x, uv->y);

        int intensity = int(0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b);

        c = '#';

        if (intensity < 16)       { c = char(250); }
        else if (intensity < 32)  { c = char(249); }
        else if (intensity < 48)  { c = char('-'); }
        else if (intensity < 64)  { c = char('^'); }
        else if (intensity < 80)  { c = char(248); }
        else if (intensity < 96)  { c = char('*'); }
        else if (intensity < 112) { c = char('='); }
        else if (intensity < 128) { c = char(240); }
        else if (intensity < 144) { c = char(246); }
        else if (intensity < 160) { c = char(237); }
        else if (intensity < 176) { c = char(234); }
        else if (intensity < 192) { c = char(247); }
        else if (intensity < 208) { c = char('#'); }
        else if (intensity < 224) { c = char(254); }
        else if (intensity < 240) { c = char(178); }
        else                      { c = char(219); }

        if (rgb.r > 85 && rgb.r <= 170)
        {
            attribute |= FOREGROUND_RED;
        }
        else if (rgb.r > 170)
        {
            attribute |= FOREGROUND_INTENSITY | FOREGROUND_RED;
        }

        if (rgb.g > 85 && rgb.g <= 170)
        {
            attribute |= FOREGROUND_GREEN;
        }
        else if (rgb.g > 170)
        {
            attribute |= FOREGROUND_INTENSITY | FOREGROUND_GREEN;
        }

        if (rgb.b > 85 && rgb.b <= 170)
        {
            attribute |= FOREGROUND_BLUE;
        }
        else if (rgb.b > 170)
        {
            attribute |= FOREGROUND_INTENSITY | FOREGROUND_BLUE;
        }
    }

    ci->Attributes = attribute;
    ci->Char.UnicodeChar = c;
}