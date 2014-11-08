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

void Swap(Vector3f& a, Vector3f& b)
{
    Vector3f t = b;
    b = a;
    a = t;
}

float Clamp(float value, float min = 0, float max = 1)
{
    return std::fmax(min, std::fmin(value, max));
}

float Interpolate(float min, float max, float gradient)
{
    return min + (max - min) * Clamp(gradient);
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

    CHAR_INFO* screenBuffer = hConsole->GetScreenBuffer();
    COORD sSize = { this->sWidth, this->sHeight };

    // Transform the vertices
    for (int i = 0; i < vertexBufferSize; i++)
    {
        positionBuffer[i] = program->VertexShader(&vertexBuffer[i]);

        vertexBuffer[i].pos = positionBuffer[i].XYZ() / positionBuffer[i].w;
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
            //return; // Clip polygon, or the entire mesh (return for entire mesh)
        }
    }

    // Rasterize & Fragment shader
    // Rasterization method primarially researched at:
    // http://blogs.msdn.com/b/davrous/archive/2013/06/21/tutorial-part-4-learning-how-to-write-a-3d-software-engine-in-c-ts-or-js-rasterization-amp-z-buffering.aspx
    for (int v = 0; v < vertexBufferSize; v += 3)
    {
        if (!validPolyBuffer[v / 3]) { continue; }

        // p1.y >= p2.y >= p3.y
        Vector3f p1 = vertexBuffer[v].pos;
        Vector3f p2 = vertexBuffer[v + 1].pos;
        Vector3f p3 = vertexBuffer[v + 2].pos;

        if (p1.y < p2.y) { Swap(p1, p2); }
        if (p2.y < p3.y) { Swap(p2, p3); }
        if (p1.y < p2.y) { Swap(p1, p2); }

        float dp1p2 = 0, dp1p3 = 0;

        if ((p2.y - p1.y) < 0)
        {
            dp1p2 = (p2.x - p1.x) / (p2.y - p1.y);
        }

        if ((p3.y - p1.y) < 0)
        {
            dp1p3 = (p3.x - p1.x) / (p3.y - p1.y);
        }

        bool right = (dp1p2 != 0) ? (dp1p2 < dp1p3) : (p2.x > p1.x);

        int sRow = int(-(p1.y * halfSHeight) + halfSHeight);
        int eRow = int(-(p3.y * halfSHeight) + halfSHeight);

        for (int row = sRow; row <= eRow; row++)
        {
            if (row < 0 || row >= sSize.Y) { continue; }

            float y = (halfSHeight - row) / halfSHeight;
            int sCol = 0;
            int eCol = 0;
            float sx = 0;
            float ex = 0;

            if (right)
            {
                // p2 is on this side of the triangle:
                //
                // p1      p1----p2
                // |\      |    /
                // | \     |   /
                // |  p2   |  /
                // | /     | /
                // |/      |/
                // p3      p3

                if (y > p2.y)
                {
                    // Currently scanning this half of the triangle:
                    //
                    // p1      p1----p2
                    // |\      |    /
                    // |*\     |   /
                    // |**p2   |  /
                    // | /     | /
                    // |/      |/
                    // p3      p3

                    float grad1 = (p1.y != p3.y) ? (y - p1.y) / (p3.y - p1.y) : 1;
                    float grad2 = (p1.y != p2.y) ? (y - p1.y) / (p2.y - p1.y) : 1;

                    sx = Interpolate(p1.x, p3.x, grad1);
                    ex = Interpolate(p1.x, p2.x, grad2);
                }
                else
                {
                    // Currently scanning this half of the triangle:
                    //
                    // p1      p1----p2
                    // |\      |****/
                    // | \     |***/
                    // |**p2   |**/
                    // |*/     |*/
                    // |/      |/
                    // p3      p3

                    float grad1 = (p1.y != p3.y) ? (y - p1.y) / (p3.y - p1.y) : 1;
                    float grad2 = (p2.y != p3.y) ? (y - p2.y) / (p3.y - p2.y) : 1;

                    sx = Interpolate(p1.x, p3.x, grad1);
                    ex = Interpolate(p2.x, p3.x, grad2);
                }
            }
            else
            {
                // p2 is on this side of the triangle (or in line):
                //
                //    p1   p2----p1
                //    /|   |    /
                //   / |   |   /
                // p2  |   |  /
                //   \ |   | /
                //    \|   |/
                //    p3   p3

                if (y > p2.y)
                {
                    // Currently scanning this half of the triangle:
                    //
                    //    p1   p2----p1
                    //    /|   |    /
                    //   /*|   |   /
                    // p2**|   |  /
                    //   \ |   | /
                    //    \|   |/
                    //    p3   p3

                    float grad1 = (p1.y != p2.y) ? (y - p1.y) / (p2.y - p1.y) : 1;
                    float grad2 = (p1.y != p3.y) ? (y - p1.y) / (p3.y - p1.y) : 1;

                    sx = Interpolate(p1.x, p2.x, grad1);
                    ex = Interpolate(p1.x, p3.x, grad2);
                }
                else
                {
                    // Currently scanning this half of the triangle:
                    //
                    //    p1   p2----p1
                    //    /|   |****/
                    //   / |   |***/
                    // p2**|   |**/
                    //   \*|   |*/
                    //    \|   |/
                    //    p3   p3

                    float grad1 = (p2.y != p3.y) ? (y - p2.y) / (p3.y - p2.y) : 1;
                    float grad2 = (p1.y != p3.y) ? (y - p1.y) / (p3.y - p1.y) : 1;

                    sx = Interpolate(p2.x, p3.x, grad1);
                    ex = Interpolate(p1.x, p3.x, grad2);
                }
            }

            sCol = int(sx * halfSWidth + halfSWidth);
            eCol = int(ex * halfSWidth + halfSWidth);

            for (int col = sCol; col < eCol; col++)
            {
                if (col < 0 || col >= sSize.X) { continue; }

                CHAR_INFO ci;
                program->FragmentShader(&ci);

                screenBuffer[(row * sSize.X) + col] = ci;
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

    this->sHeight = hConsole->GetBufferSizeAsCoord().Y;
    this->sWidth = hConsole->GetBufferSizeAsCoord().X;
    this->halfSHeight = this->sHeight / 2.0f;
    this->halfSWidth = this->sWidth / 2.0f;

    zBuffer = new float[hConsole->GetBufferSize()];
}

void GPU::SetShaderProgram(Shader* program)
{
    this->program = program;
}