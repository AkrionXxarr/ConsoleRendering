#include <limits>
#include <iomanip>

#include "GPU.h"
#include "MathClasses.h"
#include "Console.h"
#include "Shader.h"
#include "Mesh.h"

#include "TextLogger.h"

#define EPSILON 0.00001

using namespace Math;

//#define _DEBUG_LOGGER
#ifdef _DEBUG_LOGGER
TextLogger gpuLogger("GPU");
#endif

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
    positionBuffer = new Vector4f[2048];
    validPolyBuffer = new bool[2048];
}

GPU::~GPU()
{
    if (zBuffer != nullptr)
        delete[] zBuffer;
    if (vertexBuffer != nullptr)
        delete[] vertexBuffer;
    if (validPolyBuffer != nullptr)
        delete[] validPolyBuffer;
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

    #ifdef _DEBUG_LOGGER
    sStream.clear();
    sStream << "=====================================\n";
    sStream << "============ RENDER CALL ============\n";
    sStream << "=====================================\n\n";
    #endif

    #ifdef _DEBUG_LOGGER
    sStream << "------------- VERTEX SHADER -------------\n";
    #endif
    // Transform the vertices
    for (int i = 0; i < vertexBufferSize; i++)
    {
        positionBuffer[i] = program->VertexShader(&vertexBuffer[i]);

        #ifdef _DEBUG_LOGGER
        Vector4f pb = positionBuffer[i];
        sStream << std::setprecision(8) << std::fixed
            << "Before W divide: [" << pb.x << ", " << pb.y << ", " << pb.z << ", " << pb.w << "]\n";
        #endif

        vertexBuffer[i].pos = positionBuffer[i].XYZ() / positionBuffer[i].w;

        #ifdef _DEBUG_LOGGER
        Vector3f vb = vertexBuffer[i].pos;
        sStream << std::setprecision(8) << std::fixed
            << "After  W divide: [" << vb.x << ", " << vb.y << ", " << vb.z << "]\n";
        #endif
    }

    for (int v = 0; v < vertexBufferSize; v += 3)
    {
        validPolyBuffer[v / 3] = true;
        Vector4f va = positionBuffer[v];
        Vector4f vb = positionBuffer[v + 1];
        Vector4f vc = positionBuffer[v + 2];

        if (va.w <= 0.01f || vb.w < 0.01f || vc.w < 0.01f)
        {
            validPolyBuffer[v / 3] = false;
            //return; // Clip polygon, or the entire mesh
        }
    }

    #ifdef _DEBUG_LOGGER
    sStream << "\n------------- RASTERIZATION -------------\n";
    #endif
    // Rasterize & Fragment shader
    for (int v = 0; v < vertexBufferSize; v += 3)
    {
        if (!validPolyBuffer[v / 3]) { continue; }

        Vector3f va = vertexBuffer[v].pos;
        Vector3f vb = vertexBuffer[v + 1].pos;
        Vector3f vc = vertexBuffer[v + 2].pos;

        #ifdef _DEBUG_LOGGER
        float aw = positionBuffer[0].w;
        float bw = positionBuffer[1].w;
        float cw = positionBuffer[2].w;
        float dw = positionBuffer[5].w;

        if (!gotZValue)
        {
            if ((aw < 0) || (bw < 0) || (cw < 0) || (dw < 0))
            {
                sStream << "\n!!!!!! NEGATIVE W VALUE !!!!!!\n"
                    << std::setprecision(8) << std::fixed
                    << "v1: " << aw
                    << " | v2: " << bw
                    << " | v3: " << cw
                    << " | v4: " << dw
                    << "\n";
            }

            sStream << "\nVertex coordinates...\n"
                << std::setprecision(8) << std::fixed
                << "v1: (" << vertexBuffer[0].pos.x << ", " << vertexBuffer[0].pos.y << ", " << vertexBuffer[0].pos.z << ")\n"
                << "v2: (" << vertexBuffer[1].pos.x << ", " << vertexBuffer[1].pos.y << ", " << vertexBuffer[1].pos.z << ")\n"
                << "v3: (" << vertexBuffer[2].pos.x << ", " << vertexBuffer[2].pos.y << ", " << vertexBuffer[2].pos.z << ")\n"
                << "v4: (" << vertexBuffer[5].pos.x << ", " << vertexBuffer[5].pos.y << ", " << vertexBuffer[5].pos.z << ")\n"
                << "\n\nBarycentric coordinates...\n";
        }
        #endif

        for (int i = 0; i < sSize.X; i++)
        {
            for (int j = 0; j < sSize.Y; j++)
            {
                bool assignPixel = false;

                float x = (i - halfSWidth) / halfSWidth;
                float y = (halfSHeight - j) / halfSHeight;

                float u, v, w;

                Barycentric(Vector2f(x, y), va.XY(), vb.XY(), vc.XY(), u, v, w);

                Vector3f bPoint = ((va * u) + (vb * v) + (vc * w));

                if ((u < 0 - EPSILON) || (v < 0 - EPSILON) || (w < 0 - EPSILON)) { continue; }

                #ifdef _DEBUG_LOGGER
                if ((((i % 10) == 0) && ((j % 10) == 0)) && !gotZValue)
                {
                    if ((u < 0) || (v < 0) || (w < 0)) 
                        sStream << "!!";

                    sStream << std::setprecision(4) << std::fixed
                        << "screen(" << x << ", " << y << "): point("
                        << std::setprecision(8) << std::fixed 
                        << bPoint.x << ", " << bPoint.y << ", " << bPoint.z << ")\n";
                }
                #endif

                if (bPoint.z > 1) { continue; }
                if (bPoint.z < zBuffer[(j * sSize.X) + i])
                {
                    assignPixel = true;
                    zBuffer[(j * sSize.X) + i] = bPoint.z;
                }

                if (assignPixel)
                {
                    CHAR_INFO ci;
                    program->FragmentShader(&ci);

                    screenBuffer[(j * sSize.X) + i] = ci;
                }
            }
        }
        gotZValue = true;
    }

#ifdef _DEBUG_LOGGER
    sStream << "\n";
    gpuLogger.AddLine(sStream.str());
#endif
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