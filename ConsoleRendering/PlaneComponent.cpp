#include <Windows.h>

#include "PlaneComponent.h"
#include "Mesh.h"
#include "RenderingEngine.h"
#include "FlatShader.h"

using namespace Math;

Vertex vertices[] =
{
    Vertex(Vector3f(-1, 1, 0), Vector2f(0, 0), Vector3f(0, 0, 0)),
    Vertex(Vector3f(1, 1, 0), Vector2f(0, 0), Vector3f(0, 0, 0)),
    Vertex(Vector3f(-1, -1, 0), Vector2f(0, 0), Vector3f(0, 0, 0)),

    Vertex(Vector3f(-1, -1, 0), Vector2f(0, 0), Vector3f(0, 0, 0)),
    Vertex(Vector3f(1, 1, 0), Vector2f(0, 0), Vector3f(0, 0, 0)),
    Vertex(Vector3f(1, -1, 0), Vector2f(0, 0), Vector3f(0, 0, 0))
};

PlaneComponent::PlaneComponent(char c, unsigned short attribute)
{
    this->mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    this->shader = new FlatShader(c, attribute);
}

PlaneComponent::~PlaneComponent()
{
    if (this->mesh != nullptr) { delete this->mesh; }
    if (this->shader != nullptr) { delete this->shader; }
}

float counter = 0;
void PlaneComponent::Update(float delta)
{
}

void PlaneComponent::Render(RenderingEngine* renderingEngine)
{
    shader->Bind();
    shader->Update(&(this->object->transform), renderingEngine);
    mesh->Draw();
}