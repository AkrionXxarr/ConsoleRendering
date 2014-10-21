#include "MathClasses.h"

namespace Math
{
    /////////////////////////////
    // Vector3f
    //
    Vector3f Vector3f::Rotate(Vector3f axis, float angle)
    {
        return Rotate(Quaternion(axis, angle));
    }

    Vector3f Vector3f::Rotate(Quaternion rotation)
    {
        Quaternion conjugate = rotation.Conjugate();
        Quaternion w = rotation * (*this) * conjugate;

        return Vector3f(w.x, w.y, w.z);
    }

    ///////////////////////////
    // Matrix4f
    //
    Matrix4f Matrix4f::identity(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
        );

    Matrix4f Matrix4f::zero(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
        );
}; // namespace Math