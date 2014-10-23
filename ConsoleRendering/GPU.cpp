#include <limits>

#include "GPU.h"
#include "MathClasses.h"
#include "Console.h"
#include "Shader.h"
#include "Mesh.h"

#include "TextLogger.h"

#define EPSILON 0.00001

using namespace Math;

//TextLogger gpuLogger("GPU2");

void Barycentric(Vector2f p, Vector2f a, Vector2f b, Vector2f c, float& u, float& v, float& w)
{
    Vector2f v0 = b - a, v1 = c - a, v2 = p - a;

    float d00 = v0.Dot(v0);
    float d01 = v0.Dot(v1);
    float d11 = v1.Dot(v1);
    float d20 = v2.Dot(v0);
    float d21 = v2.Dot(v1);
    float denom = d00 * d11 - d01 * d01;

    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}

GPU::GPU()
{
    vertexBuffer = new Vertex[2048];
}

GPU::~GPU()
{
    if (zBuffer != nullptr)
        delete[] zBuffer;
    if (vertexBuffer != nullptr)
        delete[] vertexBuffer;
}

void GPU::DrawElements()
{
    if (program == nullptr) return;
    std::stringstream sStream;
    bool gotZValue = false;

    COORD sSize = hConsole->GetBufferSizeAsCoord();
    CHAR_INFO* screenBuffer = hConsole->GetScreenBuffer();
    float halfSWidth = sSize.X / 2.0f;
    float halfSHeight = sSize.Y / 2.0f;

    // Transform the vertices
    for (int i = 0; i < vertexBufferSize; i++)
    {
        program->VertexShader(&vertexBuffer[i]);
    }

    // Rasterize & Fragment shader
    for (int v = 0; v < vertexBufferSize; v += 3)
    {
        Vector3f va = vertexBuffer[v].pos;
        Vector3f vb = vertexBuffer[v + 1].pos;
        Vector3f vc = vertexBuffer[v + 2].pos;

        for (int i = 0; i < sSize.X; i++)
        {
            for (int j = 0; j < sSize.Y; j++)
            {
                bool assignPixel = false;

                float x = (i - halfSWidth) / halfSWidth;
                float y = (halfSHeight - j) / halfSHeight;

                float u, v, w;

                Barycentric(Vector2f(x, y), va.XY(), vb.XY(), vc.XY(), u, v, w);

                if ((u < 0) || (v < 0) || (w < 0)) { continue; }

                float z = ((va * u) + (vb * v) + (vc * w)).z;

                if (z < 0 || z > 1) { continue; }
                if (z < zBuffer[(j * sSize.X) + i])
                {
                    assignPixel = true;
                    zBuffer[(j * sSize.X) + i] = z;
                }

                if (((i == sSize.X / 2) && (j == sSize.Y / 2)) && !gotZValue)
                {
                    //sStream.clear();
                    //sStream << ": " << z << "\n";
                    //gpuLogger.AddLine(sStream.str());
                    //gotZValue = true;
                }

                if (assignPixel)
                {
                    CHAR_INFO ci;
                    program->FragmentShader(&ci);

                    screenBuffer[(j * sSize.X) + i] = ci;
                }
            }
        }
    }
}

// Create a new array for the vertex buffer as the vertices will be modified
// and we don't want to mess with the vertices coming in from the mesh
void GPU::BindVertexBuffer(Vertex* buffer, int size)
{
    this->vertexBufferSize = size;

    for (int i = 0; i < size; i++)
        this->vertexBuffer[i] = buffer[i];
}

void GPU::ClearZBuffer()
{
    // Set all zBuffer values to greatest distance possible.
    for (int i = 0; i < hConsole->GetBufferSize(); i++)
        zBuffer[i] = std::numeric_limits<float>::max();
}

void GPU::ClearBuffers(bool clearZBuffer, bool clearScreenBuffer)
{
    CHAR_INFO* screenBuffer = hConsole->GetScreenBuffer();

    for (int i = 0; i < hConsole->GetBufferSize(); i++)
    {
        if (clearZBuffer) { zBuffer[i] = std::numeric_limits<float>::max(); }
        if (clearScreenBuffer) { screenBuffer[i].Attributes = 0; screenBuffer[i].Char.UnicodeChar = ' '; }
    }
}

void GPU::SetConsoleHandle(Console* hConsole)
{
    this->hConsole = hConsole;

    zBuffer = new float[hConsole->GetBufferSize()];
}

void GPU::SetShaderProgram(Shader* program)
{
    this->program = program;
}