#pragma once

#include "MathClasses.h"

struct Vertex
{
    Vertex();
    Vertex(
        const Math::Vector3f& pos, 
        const Math::Vector2f& uv, 
        const Math::Vector3f& normal
        );
    Vertex(const Vertex& other);
    ~Vertex();

    void operator = (const Vertex& other);

    Math::Vector3f pos;
    Math::Vector2f uv;
    Math::Vector3f normal;
};

class Mesh
{
public:
    Mesh(Vertex* vertices, unsigned int numVertices);
    Mesh(const Mesh& other);
    ~Mesh();

    void Draw();

    void operator = (const Mesh& other);

public:
    Vertex* vertexBuffer;
    unsigned int bufferSize;
};