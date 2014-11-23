#include "GPU.h"
#include "Mesh.h"

using namespace Math;

Vertex::Vertex()
{
}

Vertex::Vertex(const Vector3f& pos, const Vector2f& uv = Vector2f(0, 0), const Vector3f& normal = Vector3f(0, 0, 0))
{
    this->pos = pos;
    this->uv = uv;
    this->normal = normal;
}

Vertex::Vertex(const Vertex& other)
{
    pos = other.pos;
    uv = other.uv;
    normal = other.normal;
}

Vertex::~Vertex()
{
}

void Vertex::operator = (const Vertex& other)
{
    pos = other.pos;
    uv = other.uv;
    normal = other.normal;
}



Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
    vertexBuffer = vertices;
    bufferSize = numVertices;
}

Mesh::Mesh(const Mesh& other)
{
    vertexBuffer = other.vertexBuffer;
    bufferSize = other.bufferSize;
}

Mesh::~Mesh()
{
    if (vertexBuffer != nullptr) { delete[] vertexBuffer; }
}

void Mesh::Draw()
{
    GPU::Get().BindVertexBuffer(vertexBuffer, bufferSize);
    GPU::Get().DrawElements();
}

void Mesh::operator = (const Mesh& other)
{
    vertexBuffer = other.vertexBuffer;
    bufferSize = other.bufferSize;
}