#pragma once

#include "vec3.h"
#include <cmath>

#ifndef FIXED_POINT_PRECISION
#define FIXED_POINT_PRECISION 4096
#endif

inline int fixed_sin(int angle)
{
    return std::sin(angle / static_cast<float>(FIXED_POINT_PRECISION)) * FIXED_POINT_PRECISION;
}

inline int fixed_cos(int angle)
{
    return std::cos(angle / static_cast<float>(FIXED_POINT_PRECISION)) * FIXED_POINT_PRECISION;
}

namespace Math {

struct Matrix4
{
    int m[16];

    inline Matrix4()
    {
        for (int i = 0; i < 16; ++i)
            m[i] = (i % 5 == 0) ? FIXED_POINT_PRECISION : 0;
    }

    inline static Matrix4 identity()
    {
        return Matrix4();
    }

    inline static Matrix4 multiply(const Matrix4 &a, const Matrix4 &b)
    {
        Matrix4 result;
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                int sum = 0;
                for (int k = 0; k < 4; ++k)
                {
                    sum += (a.m[row * 4 + k] * b.m[k * 4 + col]) / FIXED_POINT_PRECISION;
                }
                result.m[row * 4 + col] = sum;
            }
        }
        return result;
    }

    inline static Vec3 multiply(const Matrix4 &mat, const Vec3 &v)
    {
        int x = (mat.m[0] * v.x + mat.m[1] * v.y + mat.m[2] * v.z + mat.m[3]) / FIXED_POINT_PRECISION;
        int y = (mat.m[4] * v.x + mat.m[5] * v.y + mat.m[6] * v.z + mat.m[7]) / FIXED_POINT_PRECISION;
        int z = (mat.m[8] * v.x + mat.m[9] * v.y + mat.m[10] * v.z + mat.m[11]) / FIXED_POINT_PRECISION;
        return Vec3(x, y, z);
    }

    inline static Matrix4 translation(int tx, int ty, int tz)
    {
        Matrix4 result = identity();
        result.m[12] = tx * FIXED_POINT_PRECISION;
        result.m[13] = ty * FIXED_POINT_PRECISION;
        result.m[14] = tz * FIXED_POINT_PRECISION;
        return result;
    }

    inline static Matrix4 scale(int sx, int sy, int sz)
    {
        Matrix4 result;
        result.m[0] = sx * FIXED_POINT_PRECISION;
        result.m[5] = sy * FIXED_POINT_PRECISION;
        result.m[10] = sz * FIXED_POINT_PRECISION;
        return result;
    }

    inline static Matrix4 rotateX(int angle)
    {
        Matrix4 mat = identity();
        float rad = angle / static_cast<float>(FIXED_POINT_PRECISION);
        int cos_val = static_cast<int>(std::cos(rad) * FIXED_POINT_PRECISION);
        int sin_val = static_cast<int>(std::sin(rad) * FIXED_POINT_PRECISION);
        mat.m[5] = cos_val;
        mat.m[6] = -sin_val;
        mat.m[9] = sin_val;
        mat.m[10] = cos_val;
        return mat;
    }

    inline static Matrix4 rotateY(int angle)
    {
        Matrix4 mat = identity();
        float rad = angle / static_cast<float>(FIXED_POINT_PRECISION);
        int cos_val = static_cast<int>(std::cos(rad) * FIXED_POINT_PRECISION);
        int sin_val = static_cast<int>(std::sin(rad) * FIXED_POINT_PRECISION);
        mat.m[0] = cos_val;
        mat.m[2] = sin_val;
        mat.m[8] = -sin_val;
        mat.m[10] = cos_val;
        return mat;
    }

    inline static Matrix4 rotateZ(int angle)
    {
        Matrix4 mat = identity();
        float rad = angle / static_cast<float>(FIXED_POINT_PRECISION);
        int cos_val = static_cast<int>(std::cos(rad) * FIXED_POINT_PRECISION);
        int sin_val = static_cast<int>(std::sin(rad) * FIXED_POINT_PRECISION);
        mat.m[0] = cos_val;
        mat.m[1] = -sin_val;
        mat.m[4] = sin_val;
        mat.m[5] = cos_val;
        return mat;
    }

    inline static Matrix4 perspective(float fov_rad, float aspect, float near, float far)
    {
        Matrix4 result = {};
        float tanHalfFov = std::tan(fov_rad / 2.0f);
        result.m[0] = static_cast<int>((1.0f / (aspect * tanHalfFov)) * FIXED_POINT_PRECISION);
        result.m[5] = static_cast<int>((1.0f / tanHalfFov) * FIXED_POINT_PRECISION);
        result.m[10] = static_cast<int>(((far + near) / (near - far)) * FIXED_POINT_PRECISION);
        result.m[11] = static_cast<int>(((2.0f * far * near) / (near - far)) * FIXED_POINT_PRECISION);
        result.m[14] = -FIXED_POINT_PRECISION;
        result.m[15] = 0;
        return result;
    }

    inline static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        Matrix4 result = {};
        result.m[0] = static_cast<int>((2.0f / (right - left)) * FIXED_POINT_PRECISION);
        result.m[5] = static_cast<int>((2.0f / (top - bottom)) * FIXED_POINT_PRECISION);
        result.m[10] = static_cast<int>((-2.0f / (far - near)) * FIXED_POINT_PRECISION);
        result.m[12] = static_cast<int>((-(right + left) / (right - left)) * FIXED_POINT_PRECISION);
        result.m[13] = static_cast<int>((-(top + bottom) / (top - bottom)) * FIXED_POINT_PRECISION);
        result.m[14] = static_cast<int>((-(far + near) / (far - near)) * FIXED_POINT_PRECISION);
        result.m[15] = FIXED_POINT_PRECISION;
        return result;
    }
};

}