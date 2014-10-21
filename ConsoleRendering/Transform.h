#pragma once

#include "MathClasses.h"

class Object;

class Transform
{
public:
    Transform();
    ~Transform();

    void Rotate(Math::Vector3f axis, float angle);

    void LookAt(Math::Vector3f point, Math::Vector3f up);

    Math::Quaternion GetLookAtDirection(Math::Vector3f point, Math::Vector3f up);
    Math::Matrix4f GetTransformation();
    Math::Matrix4f GetParentMatrix();
    Math::Vector3f GetTransformedPos();
    Math::Quaternion GetTransformedRot();

public:
    Math::Vector3f pos;
    Math::Quaternion rot;
    Math::Vector3f scale;
    Object* object;
};