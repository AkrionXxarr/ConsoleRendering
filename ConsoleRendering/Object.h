#pragma once
#include <vector>

#include "Transform.h"

class Object;
class RenderingEngine;

class Component
{
public:
    Component() { }
    virtual ~Component() { }

    virtual void Update(float delta) = 0;
    virtual void Render(RenderingEngine* renderingEngine) = 0;
    
    //virtual void AddToEngine(RenderingEngine* engine);

public:
    Object* object;
};

class Object
{
public:
    Object() { }
    ~Object() 
    {
        for each (Component* component in components)
        {
            delete component;
        }
    }

    void AddChild(Object* child)
    {
        child->parent = this;
        children.push_back(child);
    }

    void AddComponent(Component* comp)
    {
        comp->object = this;
        components.push_back(comp);
    }

    void UpdateAll(float delta)
    {
        Update(delta);

        for each (Object* obj in children)
        {
            obj->UpdateAll(delta);
        }
    }

    void RenderAll(RenderingEngine* renderingEngine)
    {
        Render(renderingEngine);

        for each (Object* obj in children)
        {
            obj->Render(renderingEngine);
        }
    }

    void Update(float delta)
    {
        for each (Component* component in components)
        {
            component->Update(delta);
        }
    }

    void Render(RenderingEngine* renderingEngine)
    {
        for each (Component* component in components)
        {
            component->Render(renderingEngine);
        }
    }

public:
    Object* parent;
    Transform transform;

private:
    std::vector<Object*> children;
    std::vector<Component*> components;
};