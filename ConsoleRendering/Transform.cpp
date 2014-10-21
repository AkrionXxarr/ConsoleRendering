#include "Transform.h"
#include "Object.h"

using namespace Math;

Transform::Transform()
{
    pos = Vector3f(0, 0, 0);
    rot = Quaternion(0, 0, 0, 1);
    scale = Vector3f(1, 1, 1);
}

Transform::~Transform()
{
}

void Transform::Rotate(Vector3f axis, float angle)
{
    rot = (Quaternion(axis, angle) * rot).Normalized();
}

void Transform::LookAt(Vector3f point, Vector3f up)
{
    rot = GetLookAtDirection(point, up);
}

Quaternion Transform::GetLookAtDirection(Vector3f point, Vector3f up)
{
    return Quaternion(Matrix4f().InitRotation((point - pos).Normalized(), up));
}

Matrix4f Transform::GetTransformation()
{
    Matrix4f transMatrix = Matrix4f().InitTranslation(pos.x, pos.y, pos.z);
    Matrix4f rotMatrix = rot.ToRotationMatrix();
    Matrix4f scaleMatrix = Matrix4f().InitScale(scale.x, scale.y, scale.z);

    return transMatrix * rotMatrix * scaleMatrix;
}

Matrix4f Transform::GetParentMatrix()
{
    Matrix4f m = Matrix4f().identity;
    return m;
}

Vector3f Transform::GetTransformedPos()
{
    return GetParentMatrix().Transform(pos);
}

Quaternion Transform::GetTransformedRot()
{
    Quaternion q = Quaternion(0, 0, 0, 1);

    if (object != nullptr)
        q = object->transform.GetTransformedRot();

    return q * rot;
}