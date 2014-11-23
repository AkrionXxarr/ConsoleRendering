#include <Windows.h>

#include "PlaneComponent.h"
#include "Mesh.h"
#include "RenderingEngine.h"
#include "FlatShader.h"

using namespace Math;

Vertex g_vertices[] =
{
    Vertex(Vector3f(-1, 1, 0), Vector2f(-1, 1), Vector3f(0, 0, 0)),
    Vertex(Vector3f(1, 1, 0), Vector2f(1, 1), Vector3f(0, 0, 0)),
    Vertex(Vector3f(-1, -1, 0), Vector2f(-1, -1), Vector3f(0, 0, 0)),

    Vertex(Vector3f(-1, -1, 0), Vector2f(-1, -1), Vector3f(0, 0, 0)),
    Vertex(Vector3f(1, 1, 0), Vector2f(1, 1), Vector3f(0, 0, 0)),
    Vertex(Vector3f(1, -1, 0), Vector2f(1, -1), Vector3f(0, 0, 0))
};

PlaneComponent::PlaneComponent(Shader* shader)
{
    int g_vertSize = sizeof(g_vertices) / sizeof(g_vertices[0]);
    Vertex* vertices = new Vertex[g_vertSize];

    for (int i = 0; i < g_vertSize; i++)
        vertices[i] = g_vertices[i];

    mesh = new Mesh(vertices, g_vertSize);
    this->shader = shader;
}

void PlaneComponent::Destroy()
{
    if (mesh != nullptr) { delete mesh; }
}

float counter = 0;
void PlaneComponent::Update(float delta)
{
}

void PlaneComponent::Render(RenderingEngine* renderingEngine)
{
    shader->Bind();
    shader->Update(&(object->transform), renderingEngine);
    mesh->Draw();
}