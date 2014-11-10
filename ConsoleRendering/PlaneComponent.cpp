#include <Windows.h>

#include "PlaneComponent.h"
#include "Mesh.h"
#include "RenderingEngine.h"
#include "FlatShader.h"

#include "TextLogger.h"

using namespace Math;

#ifdef _DEBUG_LOGGER
TextLogger objLogger("Obj");
#endif

Vertex vertices[] =
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
    this->mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    this->shader = shader;
}

PlaneComponent::~PlaneComponent()
{
    if (this->mesh != nullptr) { delete this->mesh; }
}

float counter = 0;
void PlaneComponent::Update(float delta)
{
}

void PlaneComponent::Render(RenderingEngine* renderingEngine)
{
    #ifdef _DEBUG_LOGGER
    std::stringstream sStream;
    sStream << ": " << this->object->transform.pos.z << "\n";
    objLogger.AddLine(sStream.str());
    #endif

    shader->Bind();
    shader->Update(&(this->object->transform), renderingEngine);
    mesh->Draw();
}