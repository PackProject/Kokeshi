#ifndef EGG_MATH_VECTOR_H
#define EGG_MATH_VECTOR_H
#include "eggMath.h"
#include "math_types.h"
#include "types_egg.h"

namespace EGG {
struct Vector2f : nw4r::math::VEC2 {
    Vector2f() {}
    Vector2f(f32 _x, f32 _y) : VEC2(_x, _y) {}
    ~Vector2f() {}

    static const Vector2f zero;
    static const Vector2f ex;
    static const Vector2f ey;
};

Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs);
Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs);
Vector3f operator*(const Vector3f& lhs, f32 scalar);
Vector3f operator/(const Vector3f& lhs, f32 scalar);

struct Vector3f : nw4r::math::VEC3 {
    Vector3f() {}
    Vector3f(f32 _x, f32 _y, f32 _z) : VEC3(_x, _y, _z) {}
    ~Vector3f() {}

    void normalise();
    Vector3f cross(const Vector3f& other) const;

    f32 length() const {
        return Math<f32>::sqrt(squaredLength());
    }

    f32 squaredLength() const {
        return x * x + y * y + z * z;
    }

    f32 dot(const Vector3f& rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    f32 distance(const Vector3f& other) const {
        Vector3f delta = other - *this;
        return delta.length();
    }

    f32& operator()(int i) {
        return reinterpret_cast<f32*>(this)[i];
    }

    static const Vector3f zero;
    static const Vector3f ex;
    static const Vector3f ey;
    static const Vector3f ez;
};

} // namespace EGG

#endif
