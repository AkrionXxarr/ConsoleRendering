#include <Windows.h>

#include "GPU.h"
#include "Mesh.h"
#include "FlatShader.h"
#include "Transform.h"
#include "RenderingEngine.h"
#include "MathClasses.h"
#include "Camera.h"

using namespace Math;

FlatShader::FlatShader(char c, unsigned short attribute)
{
    this->c = c;
    this->attribute = attribute;
}

FlatShader::~FlatShader()
{
}

void FlatShader::Update(Transform* transform, RenderingEngine* renderingEngine)
{
    worldMatrix = transform->GetTransformation();
    MVPMatrix = renderingEngine->camera->GetViewProjection() * worldMatrix;
}

void FlatShader::VertexShader(Vertex* vertex)
{
    //vertex->pos = worldMatrix.Transform(vertex->pos);
    Vector4f t = MVPMatrix.Transform(Vector4f(vertex->pos, 1));
    vertex->pos = (t.XYZ() / t.w);
}

void FlatShader::FragmentShader(_CHAR_INFO* ci)
{
    ci->Attributes = attribute;
    ci->Char.UnicodeChar = c;
}