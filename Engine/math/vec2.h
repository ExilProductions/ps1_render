#pragma once

#include <cmath>
#include <cstdint>

#ifndef FIXED_POINT_PRECISION
#define FIXED_POINT_PRECISION 4096
#endif

namespace Math {
    struct Vec2
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

}