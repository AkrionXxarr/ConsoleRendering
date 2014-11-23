#include "GPU.h"
#include "RenderingEngine.h"
#include "Object.h"

void RenderingEngine::Render(Object* obj)
{
    GPU::Get().ClearBuffers(true, true);

    obj->RenderAll(this);
}