#include <limits>
#include <iomanip>

#include "GPU.h"
#include "MathClasses.h"
#include "Console.h"
#include "Shader.h"
#include "Mesh.h"


// TEMP
#include "TexturedShader.h"
//

#define EPSILON 0.00001

using namespace Math;

void Swap(float& a, float& b)
{
    float t = b;
    b = a;
    a = t;
}

void Swap(Vertex& a, Vertex& b)
{
    Vertex t = b;
    b = a;
    a = t;
}

void Swap(Vector3f& a, Vector3f& b)
{
    Vector3f t = b;
    b = a;
    a = t;
}

void Swap(Vector2f& a, Vector2f& b)
{
    Vector2f t = b;
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
    bool gotZValue = false;

    CHAR_INFO* screenBuffer = hConsole->GetScreenBuffer();
    COORD sSize = { sWidth, sHeight };

    // Transform the vertices
    for (int i = 0; i < vertexBufferSize; i++)
    {
        positionBuffer[i] = program->VertexShader(&vertexBuffer[i]);
        Vertex& v = vertexBuffer[i];
        v.pos = positionBuffer[i].XYZ() / positionBuffer[i].w;
    }

    // If a vertex cross the nearclip, drop the polygon
    for (int v = 0; v < vertexBufferSize; v += 3)
    {
        validPolyBuffer[v / 3] = true;
        Vector4f va = positionBuffer[v];
        Vector4f vb = positionBuffer[v + 1];
        Vector4f vc = positionBuffer[v + 2];

        if (va.w <= 0.01f || vb.w < 0.01f || vc.w < 0.01f) // Not good, hardcoded nearclip values.
        {
            //return; // Return to clip the entire mesh instead

            validPolyBuffer[v / 3] = false;
        }
    }

    // Rasterize & Fragment shader
    // Rasterization method primarially researched at:
    // http://blogs.msdn.com/b/davrous/archive/2013/06/21/tutorial-part-4-learning-how-to-write-a-3d-software-engine-in-c-ts-or-js-rasterization-amp-z-buffering.aspx
    for (int v = 0; v < vertexBufferSize; v += 3)
    {
        if (!validPolyBuffer[v / 3]) { continue; }

        // p1.y >= p2.y >= p3.y
        Vertex vert1 = vertexBuffer[v];
        Vertex vert2 = vertexBuffer[v + 1];
        Vertex vert3 = vertexBuffer[v + 2];

        float w1 = positionBuffer[v].w;
        float w2 = positionBuffer[v + 1].w;
        float w3 = positionBuffer[v + 2].w;

        if (vert1.pos.y < vert2.pos.y) { Swap(vert1, vert2); Swap(w1, w2); }
        if (vert2.pos.y < vert3.pos.y) { Swap(vert2, vert3); Swap(w2, w3); }
        if (vert1.pos.y < vert2.pos.y) { Swap(vert1, vert2); Swap(w1, w2); }

        Vector3f p1 = vert1.pos;
        Vector3f p2 = vert2.pos;
        Vector3f p3 = vert3.pos;

        Vector2f uv1 = vert1.uv;
        Vector2f uv2 = vert2.uv;
        Vector2f uv3 = vert3.uv;

		Vector2f uvw1 = uv1 / w1;
		Vector2f uvw2 = uv2 / w2;
		Vector2f uvw3 = uv3 / w3;

		float w1i = 1 / w1;
		float w2i = 1 / w2;
		float w3i = 1 / w3;

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
            int sCol = 0, eCol = 0;
            float sx = 0, ex = 0;
            float z1 = 0, z2 = 0;
            float su = 0, eu = 0, sv = 0, ev = 0;
			float swi = 0, ewi = 0;

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
                    z1 = Interpolate(p1.z, p3.z, grad1);
                    z2 = Interpolate(p1.z, p2.z, grad2);

                    su = Interpolate(uvw1.x, uvw3.x, grad1);
                    eu = Interpolate(uvw1.x, uvw2.x, grad2);
                    sv = Interpolate(uvw1.y, uvw3.y, grad1);
                    ev = Interpolate(uvw1.y, uvw2.y, grad2);

					swi = Interpolate(w1i, w3i, grad1);
					ewi = Interpolate(w1i, w2i, grad2);
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
                    z1 = Interpolate(p1.z, p3.z, grad1);
                    z2 = Interpolate(p2.z, p3.z, grad2);

                    su = Interpolate(uvw1.x, uvw3.x, grad1);
                    eu = Interpolate(uvw2.x, uvw3.x, grad2);
                    sv = Interpolate(uvw1.y, uvw3.y, grad1);
                    ev = Interpolate(uvw2.y, uvw3.y, grad2);

					swi = Interpolate(w1i, w3i, grad1);
					ewi = Interpolate(w2i, w3i, grad2);
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
                    z1 = Interpolate(p1.z, p2.z, grad1);
                    z2 = Interpolate(p1.z, p3.z, grad2);

                    su = Interpolate(uvw1.x, uvw2.x, grad1);
                    eu = Interpolate(uvw1.x, uvw3.x, grad2);
                    sv = Interpolate(uvw1.y, uvw2.y, grad1);
                    ev = Interpolate(uvw1.y, uvw3.y, grad2);

					swi = Interpolate(w1i, w2i, grad1);
					ewi = Interpolate(w1i, w3i, grad2);
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
                    z1 = Interpolate(p2.z, p3.z, grad1);
                    z2 = Interpolate(p1.z, p3.z, grad2);

                    su = Interpolate(uvw2.x, uvw3.x, grad1);
                    eu = Interpolate(uvw1.x, uvw3.x, grad2);
                    sv = Interpolate(uvw2.y, uvw3.y, grad1);
                    ev = Interpolate(uvw1.y, uvw3.y, grad2);

					swi = Interpolate(w2i, w3i, grad1);
					ewi = Interpolate(w1i, w3i, grad2);
                }
            }

            sCol = int(sx * halfSWidth + halfSWidth);
            eCol = int(ex * halfSWidth + halfSWidth);

            for (int col = sCol; col < eCol; col++)
            {
                if (col < 0 || col >= sSize.X) { continue; }
                int index = (row * sSize.X) + col;

                float x = (col - halfSWidth) / halfSWidth;
                float grad = (x - sx) / (ex - sx);

                float z = Interpolate(z1, z2, grad);

				float u = Interpolate(su, eu, grad);
				float v = Interpolate(sv, ev, grad);
				float wi = Interpolate(swi, ewi, grad);

                if (z < zBuffer[index])
                {
                    zBuffer[index] = z;
                    CHAR_INFO ci;

                    program->FragmentShader(&ci, &Vector2f(u / wi, v / wi));

                    screenBuffer[index] = ci;
                }
            }
        }
    }
}

// Create a new array for the vertex buffer as the vertices will be modified
// and we don't want to mess with the vertices coming in from the mesh
void GPU::BindVertexBuffer(Vertex* buffer, int size)
{
    vertexBufferSize = size;

    for (int i = 0; i < size; i++)
        vertexBuffer[i] = buffer[i];
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

    sHeight = hConsole->GetBufferSizeAsCoord().Y;
    sWidth = hConsole->GetBufferSizeAsCoord().X;
    halfSHeight = sHeight / 2.0f;
    halfSWidth = sWidth / 2.0f;

    zBuffer = new float[hConsole->GetBufferSize()];
}

void GPU::SetShaderProgram(Shader* program)
{
    this->program = program;
}