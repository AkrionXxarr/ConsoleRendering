#pragma once

class Console;
class Shader;
struct Vertex;

namespace Math
{
    class Vector4f;
}; // namespace Math

// This class should simulate GPU functions
class GPU
{
public:
    static GPU& Get()
    {
        static GPU instance;
        return instance;
    }

    void DrawElements();

    void BindVertexBuffer(Vertex* buffer, int size);

    void ClearZBuffer();
    void ClearBuffers(bool zBuffer, bool screenBuffer);

    void SetConsoleHandle(Console* hConsole);
    void SetShaderProgram(Shader* program);

private:
    GPU();
    ~GPU();

    GPU(GPU const& other);
    void operator = (GPU const& other);

private:
    Console* hConsole;
    Shader* program;
    Vertex* vertexBuffer;
    Math::Vector4f* positionBuffer;
    int vertexBufferSize;
    float* zBuffer;
};