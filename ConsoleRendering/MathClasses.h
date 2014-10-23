#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace Math
{
    class Vector2f;
    class Vector3f;
    class Vector4f;
    class Quaternion;
    class Matrix4f;

    /////////////////////////////////
    // Vector2f
    //
    class Vector2f
    {
    public:
        Vector2f()
        {
            this->x = 0; this->y = 0;
        }
        Vector2f(float x, float y)
        {
            this->x = x; this->y = y;
        }
        Vector2f(const Vector2f& other)
        {
            this->x = other.x; this->y = other.y;
        }

        ~Vector2f() { }

        float Length() { return sqrt(x * x + y * y); }

        float Dot(Vector2f v) { return x * v.x + y * v.y; }
        float Cross(Vector2f v) { return (x * v.y) - (y * v.x); }

        Vector2f Rotate(float angle)
        {
            float rad = angle * float(M_PI / 180.0);
            float cosine = cos(rad);
            float sine = sin(rad);

            return Vector2f((x * cosine - y * sine), (x * sine + y * cosine));
        }

        Vector2f Lerp(Vector2f dest, float factor)
        {
            return (((dest - *this) * factor) + *this);
        }

        Vector2f Normalized()
        {
            float length = Length();

            return Vector2f(x / length, y / length);
        }

        float Max()
        {
            return fmax(x, y);
        }

        float Min()
        {
            return fmin(x, y);
        }

        Vector2f Abs()
        {
            return Vector2f(fabs(x), fabs(y));
        }

        void operator = (const Vector2f& v) { this->x = v.x; this->y = v.y; }

        bool operator == (Vector2f& v) { return (x == v.x) && (y == v.y); }
        Vector2f operator - () { return Vector2f(-x, -y); }

        Vector2f operator - (Vector2f& v) { return Vector2f(x - v.x, y - v.y); }
        Vector2f operator + (Vector2f& v) { return Vector2f(x + v.x, y + v.y); }

        Vector2f operator * (Vector2f& v) { return Vector2f(x * v.x, y * v.y); }
        Vector2f operator / (Vector2f& v) { return Vector2f(x / v.x, y / v.y); }

        Vector2f operator * (float f) { return Vector2f(x * f, y * f); }
        Vector2f operator / (float f) { return Vector2f(x / f, y / f); }

    public:
        float x, y;
    };


    ///////////////////////////////
    // Vector3f
    //
    class Vector3f
    {
    public:
        Vector3f()
        {
            this->x = 0; this->y = 0; this->z = 0;
        }
        Vector3f(float x, float y, float z)
        {
            this->x = x; this->y = y; this->z = z;
        }
        Vector3f(Vector2f v, float z)
        {
            this->x = v.x; this->y = v.y; this->z = z;
        }
        Vector3f(const Vector3f& other)
        {
            this->x = other.x; this->y = other.y; this->z = other.z;
        }

        ~Vector3f() { }

        float Length() { return sqrt(x * x + y * y + z * z); }

        float Dot(Vector3f v) { return x * v.x + y * v.y + z * v.z; }
        Vector3f Cross(Vector3f v)
        {
            float i = y * v.z - z * v.y;
            float j = z * v.x - x * v.z;
            float k = x * v.y - y * v.x;

            return Vector3f(i, j, k);
        }

        /*
        Vector3f Rotate(float angle)
        {
        float rad = angle * (M_PI / 180.0);
        float cosine = cos(rad);
        float sine = sin(rad);

        return Vector3f((x * cosine - y * sine), (x * sine + y * cosine));
        }
        */

        Vector3f Rotate(Vector3f axis, float angle);
        Vector3f Rotate(Quaternion rotation);

        Vector3f Lerp(Vector3f dest, float factor)
        {
            return (((dest - *this) * factor) + *this);
        }

        Vector3f Normalized()
        {
            float length = Length();

            return Vector3f(x / length, y / length, z / length);
        }

        float Max()
        {
            return fmax(x, fmax(y, z));
        }

        float Min()
        {
            return fmin(x, fmin(y, z));
        }

        Vector3f Abs()
        {
            return Vector3f(fabs(x), fabs(y), fabs(z));
        }


        Vector2f XY() { return Vector2f(x, y); }
        Vector2f YX() { return Vector2f(y, x); }
        Vector2f XZ() { return Vector2f(x, z); }
        Vector2f ZX() { return Vector2f(z, x); }
        Vector2f YZ() { return Vector2f(y, z); }
        Vector2f ZY() { return Vector2f(z, y); }


        void operator = (const Vector3f& v) { this->x = v.x; this->y = v.y; this->z = v.z; }

        bool operator == (Vector3f v) { return (x == v.x) && (y == v.y) && (z == v.z); }
        Vector3f operator - () { return Vector3f(-x, -y, -z); }

        Vector3f operator - (Vector3f v) { return Vector3f(x - v.x, y - v.y, z - v.z); }
        Vector3f operator + (Vector3f v) { return Vector3f(x + v.x, y + v.y, z + v.z); }

        Vector3f operator * (Vector3f v) { return Vector3f(x * v.x, y * v.y, z * v.z); }
        Vector3f operator / (Vector3f v) { return Vector3f(x / v.x, y / v.y, z / v.z); }

        Vector3f operator * (float f) { return Vector3f(x * f, y * f, z * f); }
        Vector3f operator / (float f) { return Vector3f(x / f, y / f, z / f); }

    public:
        float x, y, z;
    };

    ///////////////////////////
    // Vector4f
    //
    class Vector4f
    {
    public:
        Vector4f()
        {
            this->x = 0; this->y = 0; this->z = 0; this->w = 0;
        }
        Vector4f(float x, float y, float z, float w)
        {
            this->x = x; this->y = y; this->z = z; this->w = w;
        }
        Vector4f(Vector3f v, float w)
        {
            this->x = v.x; this->y = v.y; this->z = v.z; this->w = w;
        }
        Vector4f(const Vector4f& other)
        {
            this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
        }

        ~Vector4f() { }

        float Length() { return sqrt(x * x + y * y + z * z + w * w); }

        /*
        Vector3f Rotate(float angle)
        {
        float rad = angle * (M_PI / 180.0);
        float cosine = cos(rad);
        float sine = sin(rad);

        return Vector3f((x * cosine - y * sine), (x * sine + y * cosine));
        }
        */

        Vector4f Lerp(Vector4f dest, float factor)
        {
            return (((dest - *this) * factor) + *this);
        }

        Vector4f Normalized()
        {
            float length = Length();

            return Vector4f(x / length, y / length, z / length, w / length);
        }

        float Max()
        {
            return fmax(x, fmax(y, fmax(z, w)));
        }

        float Min()
        {
            return fmin(x, fmin(y, fmin(z, w)));
        }

        Vector4f Abs()
        {
            return Vector4f(fabs(x), fabs(y), fabs(z), fabs(w));
        }


        Vector3f XYZ() { return Vector3f(x, y, z); }
        //Vector3f YXZ() { return Vector3f(y, x, z); }
        //Vector3f YZX() { return Vector3f(y, z, x); }

        bool operator == (Vector4f v) { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
        Vector4f operator - () { return Vector4f(-x, -y, -z, -w); }

        Vector4f operator - (Vector4f v) { return Vector4f(x - v.x, y - v.y, z - v.z, w - v.w); }
        Vector4f operator + (Vector4f v) { return Vector4f(x + v.x, y + v.y, z + v.z, w + v.w); }

        Vector4f operator * (Vector4f v) { return Vector4f(x * v.x, y * v.y, z * v.z, w * v.w); }
        Vector4f operator / (Vector4f v) { return Vector4f(x / v.x, y / v.y, z / v.z, w / v.w); }

        Vector4f operator * (float f) { return Vector4f(x * f, y * f, z * f, w * f); }
        Vector4f operator / (float f) { return Vector4f(x / f, y / f, z / f, w / f); }

    public:
        float x, y, z, w;
    };



    /////////////////////////////////
    // Matrix4f
    //
    class Matrix4f
    {
    public:
        Matrix4f()
        {
            this->matrix = new float[16];
            for (int i = 0; i < 16; i++) { matrix[i] = 0; }
        }
        Matrix4f(float xx, float xy, float xz, float xw,
                 float yx, float yy, float yz, float yw,
                 float zx, float zy, float zz, float zw,
                 float wx, float wy, float wz, float ww)
        {
            this->matrix = new float[16];
            M(0, 0) = xx; M(0, 1) = xy; M(0, 2) = xz; M(0, 3) = xw;
            M(1, 0) = yx; M(1, 1) = yy; M(1, 2) = yz; M(1, 3) = yw;
            M(2, 0) = zx; M(2, 1) = zy; M(2, 2) = zz; M(2, 3) = zw;
            M(3, 0) = wx; M(3, 1) = wy; M(3, 2) = wz; M(3, 3) = ww;
        }
        Matrix4f(const Matrix4f& other)
        {
            this->matrix = new float[16];
            for (int i = 0; i < 16; i++) { this->matrix[i] = other.matrix[i]; }
        }

        ~Matrix4f()
        {
            delete[] this->matrix;
        }

        float& M(int row, int col) { return matrix[(row * 4) + col]; }

        Matrix4f& InitTranslation(float x, float y, float z)
        {
            *this = Matrix4f::identity;
            M(0, 3) = x;
            M(1, 3) = y;
            M(2, 3) = z;
            return *this;
        }

        Matrix4f& InitRotation(Vector3f forward, Vector3f up)
        {
            Vector3f f = forward.Normalized();
            Vector3f r = up.Normalized();
            r = r.Cross(f);

            Vector3f u = f.Cross(r);

            return InitRotation(f, u, r);
        }

        Matrix4f& InitRotation(Vector3f f, Vector3f u, Vector3f r)
        {
            this->Set(
                r.x, r.y, r.z, 0,
                u.x, u.y, u.z, 0,
                f.x, f.y, f.z, 0,
                  0,   0,   0, 1
                );

            return *this;
        }

        Matrix4f& InitScale(float x, float y, float z)
        {
            *this = Matrix4f::identity;
            M(0, 0) = x;
            M(1, 1) = y;
            M(2, 2) = z;

            float zNear = 0;
            return *this;
        }

        Matrix4f& InitPerspective(float fov, float aspect, float zNear, float zFar)
        {
            float rad = float(M_PI / 180.0) * fov;
            float tanHalfFOV = (float)tanf(rad / 2);
            float zRange = zFar - zNear;

            this->Set(
                1 / (tanHalfFOV * aspect), 0,              0,                        0,
                0,                         1 / tanHalfFOV, 0,                        0,
                0,                         0,              zFar / zRange,            -zFar * zNear / zRange,
                0,                         0,              1,                        0
                );

            return *this;
        }

        Matrix4f& InitOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
        {
            float width = right - left;
            float height = top - bottom;
            float depth = zFar - zNear;

            this->Set(
                2 / width, 0,          0,         -(right + left) / width,
                0,         2 / height, 0,         -(top + bottom) / height,
                0,         0,         -2 / depth, -(zNear + zFar) / depth,
                0,         0,          0,         1
                );

            return *this;
        }

        Vector3f Transform(Vector3f v)
        {
            return Vector3f(
                M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z + M(0, 3),
                M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z + M(1, 3),
                M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z + M(2, 3)
                );
        }

        Vector4f Transform(Vector4f v)
        {
            return Vector4f(
                M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z + M(0, 3) * v.w,
                M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z + M(1, 3) * v.w,
                M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z + M(2, 3) * v.w,
                M(3, 0) * v.x + M(3, 1) * v.y + M(3, 2) * v.z + M(3, 3) * v.w
                );
        }

        void operator = (const Matrix4f& m)
        {
            for (int i = 0; i < 16; i++) { this->matrix[i] = m.matrix[i]; }
        }

        Matrix4f operator * (Matrix4f& m)
        {
            Matrix4f res;

            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 4; col++)
                {
                    res.M(row, col) =
                            M(row, 0) * m.M(0, col) +
                            M(row, 1) * m.M(1, col) +
                            M(row, 2) * m.M(2, col) +
                            M(row, 3) * m.M(3, col);
                }
            }

            return res;
        }


        void Set(float xx, float xy, float xz, float xw,
                 float yx, float yy, float yz, float yw,
                 float zx, float zy, float zz, float zw,
                 float wx, float wy, float wz, float ww)
        {
            M(0, 0) = xx; M(0, 1) = xy; M(0, 2) = xz; M(0, 3) = xw;
            M(1, 0) = yx; M(1, 1) = yy; M(1, 2) = yz; M(1, 3) = yw;
            M(2, 0) = zx; M(2, 1) = zy; M(2, 2) = zz; M(2, 3) = zw;
            M(3, 0) = wx; M(3, 1) = wy; M(3, 2) = wz; M(3, 3) = ww;
        }
    public:
        float* matrix;

    public:
        static Matrix4f identity;
        static Matrix4f zero;
    };

    //////////////////////////////
    // Quaternion
    //
    class Quaternion
    {
    public:
        Quaternion()
        {
            this->x = 0; this->y = 0; this->z = 0; this->w = 1;
        }
        Quaternion(float x, float y, float z, float w)
        {
            this->x = x; this->y = y; this->z = z; this->w = w;
        }
        Quaternion(Vector4f v)
        {
            this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w;
        }
        Quaternion(Vector3f axis, float angle)
        {
            float sinHalfAngle = sinf(angle / 2);
            float cosHalfAngle = cosf(angle / 2);

            this->x = axis.x * sinHalfAngle;
            this->y = axis.y * sinHalfAngle;
            this->z = axis.z * sinHalfAngle;
            this->w = cosHalfAngle;
        }
        // From Ken Shoemake's "Quaternion Calculus and Fast Animation" article
        Quaternion(Matrix4f rot)
        {
            float trace = rot.M(0, 0) + rot.M(1, 1) + rot.M(2, 2);

            if (trace > 0)
            {
                float s = 0.5f / sqrt(trace + 1.0f);
                w = 0.25f / s;
                x = (rot.M(1, 2) - rot.M(2, 1)) * s;
                y = (rot.M(2, 0) - rot.M(0, 2)) * s;
                z = (rot.M(0, 1) - rot.M(1, 0)) * s;
            }
            else
            {
                if ((rot.M(0, 0) > rot.M(1, 1)) && (rot.M(0, 0) > rot.M(2, 2)))
                {
                    float s = 2.0f * sqrt(1.0f + rot.M(0, 0) - rot.M(1, 1) - rot.M(2, 2));
                }
                else if (rot.M(1, 1) > rot.M(2, 2))
                {
                    float s = 2.0f * sqrt(1.0f + rot.M(1, 1) - rot.M(0, 0) - rot.M(2, 2));
                    w = (rot.M(2, 0) - rot.M(0, 2)) / s;
                    x = (rot.M(1, 0) + rot.M(0, 1)) / s;
                    y = 0.25f * s;
                    z = (rot.M(2, 1) + rot.M(1, 2)) / s;
                }
                else
                {
                    float s = 2.0f * sqrt(1.0f + rot.M(2, 2) - rot.M(0, 0) - rot.M(1, 1));
                    w = (rot.M(0, 1) - rot.M(1, 0)) / s;
                    x = (rot.M(2, 0) + rot.M(0, 2)) / s;
                    y = (rot.M(1, 2) + rot.M(2, 1)) / s;
                    z = 0.25f * s;
                }
            }

            *this = this->Normalized();
        }
        Quaternion(const Quaternion& other)
        {
            this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
        }
        ~Quaternion() { }

        float Length()
        {
            return sqrt(x * x + y * y + z * z + w * w);
        }

        Quaternion Normalized()
        {
            float length = Length();
            return Quaternion(x / length, y / length, z / length, w / length);
        }

        Quaternion Conjugate()
        {
            return Quaternion(-x, -y, -z, w);
        }

        float Dot(Quaternion q)
        {
            return x * q.x + y * q.y + z * q.z + w * q.w;
        }

        Quaternion NLerp(Quaternion dest, float lerpFactor, bool shortest)
        {
            Quaternion correctedDest = dest;

            if (shortest && this->Dot(dest) < 0)
                correctedDest = Quaternion(-dest.x, -dest.y, -dest.z, -dest.w);

            //return (correctedDest - *this * lerpFactor + *this).Normalized();
        }

        Matrix4f ToRotationMatrix()
        {
            Vector3f forward = Vector3f(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
            Vector3f up = Vector3f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
            Vector3f right = Vector3f(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));

            //Vector3f forward = Vector3f(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
            //Vector3f up = Vector3f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
            //Vector3f right = Vector3f(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));

            return Matrix4f().InitRotation(forward, up, right);
        }

        Vector3f GetForward()
        {
            return Vector3f(0, 0, 1).Rotate(*this);
        }

        Vector3f GetBack()
        {
            return Vector3f(0, 0, -1).Rotate(*this);
        }

        Vector3f GetUp()
        {
            return Vector3f(0, 1, 0).Rotate(*this);
        }

        Vector3f GetDown()
        {
            return Vector3f(0, -1, 0).Rotate(*this);
        }

        Vector3f GetRight()
        {
            return Vector3f(1, 0, 0).Rotate(*this);
        }

        Vector3f GetLeft()
        {
            return Vector3f(-1, 0, 0).Rotate(*this);
        }

        bool operator == (Quaternion q) { return (x == q.x) && (y == q.y) && (z == q.z) && (w = q.w); }

        Quaternion operator - (Quaternion q) { return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w); }
        Quaternion operator + (Quaternion q) { return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w); }

        Quaternion operator * (float f) { return Quaternion(x * f, y * f, z * f, w * f); }
        Quaternion operator / (float f) { return Quaternion(x / f, y / f, z / f, w / f); }

        Quaternion operator * (Quaternion q)
        {
            float i = x * q.w + w * q.x + y * q.z - z * q.y;
            float j = y * q.w + w * q.y + z * q.x - x * q.z;
            float k = z * q.w + w * q.z + x * q.y - y * q.x;
            float h = w * q.w - x * q.x - y * q.y - z * q.z;

            return Quaternion(i, j, k, h);
        }

        Quaternion operator * (Vector3f v)
        {
            float i = w * v.x + y * v.z + z * v.y;
            float j = w * v.y + z * v.x - x * v.z;
            float k = w * v.z + x * v.y - y * v.x;
            float h = -x * v.x - y * v.y - z * v.z;

            return Quaternion(i, j, k, h);
        }

    public:
        float x, y, z, w;
    };
}; // namespace Math