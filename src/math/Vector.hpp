#pragma once

#include <cmath>

namespace Vector {

class Vec2D {
public:
    float x = 0.0f;
    float y = 0.0f;

    Vec2D() = default;
    Vec2D(float px, float py) : x(px), y(py) {}

    // ---- Math operators ----
    Vec2D operator+(const Vec2D& o) const { return Vec2D(x + o.x, y + o.y); }
    Vec2D operator-(const Vec2D& o) const { return Vec2D(x - o.x, y - o.y); }
    Vec2D operator*(float s) const        { return Vec2D(x * s, y * s); }
    Vec2D operator/(float s) const        { return Vec2D(x / s, y / s); }
    Vec2D operator-() const               { return Vec2D(-x, -y); }

    Vec2D& operator+=(const Vec2D& o) { x += o.x; y += o.y; return *this; }
    Vec2D& operator-=(const Vec2D& o) { x -= o.x; y -= o.y; return *this; }
    Vec2D& operator*=(float s)        { x *= s;   y *= s;   return *this; }
    Vec2D& operator/=(float s)        { x /= s;   y /= s;   return *this; }

    bool operator==(const Vec2D& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Vec2D& o) const { return !(*this == o); }

    // ---- Vector functions ----
    float lengthSquared() const { return x * x + y * y; }
    float length() const        { return std::sqrt(lengthSquared()); }
    float dot(const Vec2D& o) const { return x * o.x + y * o.y; }
    float distanceTo(const Vec2D& o) const { return (*this - o).length(); }

    // A copy of this vector with length 1 (zero vector stays zero).
    Vec2D normalized() const {
        float len = length();
        return len > 0.0f ? *this / len : Vec2D();
    }
    void normalize() { *this = normalized(); }
};

// Lets you write 2.0f * v as well as v * 2.0f
inline Vec2D operator*(float s, const Vec2D& v) { return v * s; }


class Vec3D {
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3D() = default;
    Vec3D(float px, float py, float pz) : x(px), y(py), z(pz) {}

    // ---- Math operators ----
    Vec3D operator+(const Vec3D& o) const { return Vec3D(x + o.x, y + o.y, z + o.z); }
    Vec3D operator-(const Vec3D& o) const { return Vec3D(x - o.x, y - o.y, z - o.z); }
    Vec3D operator*(float s) const        { return Vec3D(x * s, y * s, z * s); }
    Vec3D operator/(float s) const        { return Vec3D(x / s, y / s, z / s); }
    Vec3D operator-() const               { return Vec3D(-x, -y, -z); }

    Vec3D& operator+=(const Vec3D& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec3D& operator-=(const Vec3D& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vec3D& operator*=(float s)        { x *= s;   y *= s;   z *= s;   return *this; }
    Vec3D& operator/=(float s)        { x /= s;   y /= s;   z /= s;   return *this; }

    bool operator==(const Vec3D& o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const Vec3D& o) const { return !(*this == o); }

    // ---- Vector functions ----
    float lengthSquared() const { return x * x + y * y + z * z; }
    float length() const        { return std::sqrt(lengthSquared()); }
    float dot(const Vec3D& o) const { return x * o.x + y * o.y + z * o.z; }
    float distanceTo(const Vec3D& o) const { return (*this - o).length(); }

    // The vector that is perpendicular to both (needed for 3D lighting and cameras later).
    Vec3D cross(const Vec3D& o) const {
        return Vec3D(y * o.z - z * o.y,
                     z * o.x - x * o.z,
                     x * o.y - y * o.x);
    }

    Vec3D normalized() const {
        float len = length();
        return len > 0.0f ? *this / len : Vec3D();
    }
    void normalize() { *this = normalized(); }
};

inline Vec3D operator*(float s, const Vec3D& v) { return v * s; }

} // namespace Vector