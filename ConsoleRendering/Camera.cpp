#include "Camera.h"

using namespace Math;


Camera::Camera(float fov, float aspect, float zNear, float zFar)
{
    this->projection = Math::Matrix4f().InitPerspective(fov, aspect, zNear, zFar);
}

Matrix4f Camera::GetViewProjection()
{
    Matrix4f rot = object->transform.GetTransformedRot().Conjugate().ToRotationMatrix();
    Vector3f pos = -(object->transform.GetTransformedPos());

    Matrix4f trans = Matrix4f().InitTranslation(pos.x, pos.y, pos.z);

    return projection * rot * trans;
}