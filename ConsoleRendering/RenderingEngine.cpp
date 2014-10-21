#include "GPU.h"
#include "RenderingEngine.h"
#include "Object.h"

void RenderingEngine::Render(Object* obj)
{
    //GPU::Get().ClearZBuffer();
    GPU::Get().ClearBuffers(true, true);

    obj->RenderAll(this);
}