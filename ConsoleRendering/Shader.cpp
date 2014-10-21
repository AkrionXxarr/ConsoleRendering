#include "GPU.h"
#include "Shader.h"

void Shader::Bind()
{
    GPU::Get().SetShaderProgram(this);
}