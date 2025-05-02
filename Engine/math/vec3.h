#pragma once

#include <cmath>
#include <cstdint>
using std::sqrt;

#ifndef FIXED_POINT_PRECISION
#define FIXED_POINT_PRECISION 4096
#endif

namespace Math {
    struct Vec3
{
    int x, y, z;

    inline Vec3(int x_ = 0, int y_ = 0, int z_ = 0) : x(x_), y(y_), z(z_) {}

    inline Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    inline Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    inline Vec3 operator*(int scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    inline Vec3 operator/(int scalar) const
    {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    inline Vec3 &operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline Vec3 &operator-=(const Vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline static int dot(const Vec3 &a, const Vec3 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline static Vec3 cross(const Vec3 &a, const Vec3 &b)
    {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
    }

    inline float length() const
    {
        return std::sqrt(static_cast<float>(x * x + y * y + z * z));
    }

    inline Vec3 normalized() const
    {
        float len = length();
        return len == 0.0f ? *this : Vec3(static_cast<int>(x / len), static_cast<int>(y / len), static_cast<int>(z / len));
    }

    inline float distance(const Vec3 &other) const
    {
        return std::sqrt(static_cast<float>((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z)));
    }

    inline float angle(const Vec3 &other) const
    {
        float dotProduct = dot(*this, other);
        float lengths = length() * other.length();
        return std::acos(dotProduct / lengths);
    }

    inline Vec3 reflect(const Vec3 &normal) const
    {
        int dotProduct = dot(*this, normal);
        return *this - normal * (dotProduct * 2);
    }

    inline Vec3 refract(const Vec3 &normal, float ior) const
    {
        float cosi = -dot(*this, normal);
        float eta = 1.0f / ior;
        float k = 1.0f - eta * eta * (1.0f - cosi * cosi);struct Vec2
        {
            int x, y;
        
            inline Vec2(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
        
            inline Vec2 operator+(const Vec2 &other) const
            {
                return Vec2(x + other.x, y + other.y);
            }
        
            inline Vec2 operator-(const Vec2 &other) const
            {
                return Vec2(x - other.x, y - other.y);
            }
        
            inline Vec2 operator*(int scalar_fixed) const
            {
                return Vec2((x * scalar_fixed) / FIXED_POINT_PRECISION,
                            (y * scalar_fixed) / FIXED_POINT_PRECISION);
            }
        
            inline Vec2 operator/(int scalar_fixed) const
            {
                return Vec2((x * FIXED_POINT_PRECISION) / scalar_fixed,
                            (y * FIXED_POINT_PRECISION) / scalar_fixed);
            }
        
            inline Vec2 &operator+=(const Vec2 &other)
            {
                x += other.x;
                y += other.y;
                return *this;
            }
        
            inline Vec2 &operator-=(const Vec2 &other)
            {
                x -= other.x;
                y -= other.y;
                return *this;
            }
        
            inline static int dot(const Vec2 &a, const Vec2 &b)
            {
                return a.x * b.x + a.y * b.y;
            }
        
            inline static int cross(const Vec2 &a, const Vec2 &b)
            {
                return a.x * b.y - a.y * b.x;
            }
        
            inline int length_squared() const
            {
                return x * x + y * y;
            }
        
            inline int length() const
            {
                return static_cast<int>(std::sqrt(static_cast<float>(length_squared())));
            }
        
            inline Vec2 normalized() const
            {
                int len = length();
                if (len == 0) return *this;
                return Vec2((x * FIXED_POINT_PRECISION) / len,
                            (y * FIXED_POINT_PRECISION) / len);
            }
        
            inline int distance_squared(const Vec2 &other) const
            {
                int dx = x - other.x;
                int dy = y - other.y;
                return dx * dx + dy * dy;
            }
        
            inline int distance(const Vec2 &other) const
            {
                return static_cast<int>(std::sqrt(static_cast<float>(distance_squared(other))));
            }
        
            inline static Vec2 zero() { return Vec2(0, 0); }
        };
        
        if (k < 0.0f)
            return Vec3(0, 0, 0);
        return *this * eta + normal * (eta * cosi - std::sqrt(k));
    }

    inline Vec3 project(const Vec3 &other) const
    {
        float dotProduct = dot(*this, other);
        float lenSquared = other.x * other.x + other.y * other.y + other.z * other.z;
        return other * (dotProduct / lenSquared);
    }
};

}