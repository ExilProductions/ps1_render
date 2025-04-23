#include "renderer.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

std::vector<Triangle> create_cube(int size = 4096)
{
    std::vector<Triangle> triangles;

    Vec3 v0(-size, -size, -size);
    Vec3 v1(size, -size, -size);
    Vec3 v2(size, size, -size);
    Vec3 v3(-size, size, -size);
    Vec3 v4(-size, -size, size);
    Vec3 v5(size, -size, size);
    Vec3 v6(size, size, size);
    Vec3 v7(-size, size, size);

    Vec2 t0(0.0f, 0.0f);
    Vec2 t1(1.0f, 0.0f);
    Vec2 t2(1.0f, 1.0f);
    Vec2 t3(0.0f, 1.0f);

    Triangle tri1;
    tri1.v0 = v4;
    tri1.v1 = v5;
    tri1.v2 = v6;
    tri1.uv0 = t0;
    tri1.uv1 = t1;
    tri1.uv2 = t2;
    tri1.color = Color(255, 0, 0);
    triangles.push_back(tri1);

    Triangle tri2;
    tri2.v0 = v4;
    tri2.v1 = v6;
    tri2.v2 = v7;
    tri2.uv0 = t0;
    tri2.uv1 = t2;
    tri2.uv2 = t3;
    tri2.color = Color(255, 0, 0);
    triangles.push_back(tri2);

    Triangle tri3;
    tri3.v0 = v1;
    tri3.v1 = v0;
    tri3.v2 = v3;
    tri3.uv0 = t0;
    tri3.uv1 = t1;
    tri3.uv2 = t2;
    tri3.color = Color(0, 255, 0);
    triangles.push_back(tri3);

    Triangle tri4;
    tri4.v0 = v1;
    tri4.v1 = v3;
    tri4.v2 = v2;
    tri4.uv0 = t0;
    tri4.uv1 = t2;
    tri4.uv2 = t3;
    tri4.color = Color(0, 255, 0);
    triangles.push_back(tri4);

    Triangle tri5;
    tri5.v0 = v3;
    tri5.v1 = v7;
    tri5.v2 = v6;
    tri5.uv0 = t0;
    tri5.uv1 = t1;
    tri5.uv2 = t2;
    tri5.color = Color(0, 0, 255);
    triangles.push_back(tri5);

    Triangle tri6;
    tri6.v0 = v3;
    tri6.v1 = v6;
    tri6.v2 = v2;
    tri6.uv0 = t0;
    tri6.uv1 = t2;
    tri6.uv2 = t3;
    tri6.color = Color(0, 0, 255);
    triangles.push_back(tri6);

    Triangle tri7;
    tri7.v0 = v0;
    tri7.v1 = v1;
    tri7.v2 = v5;
    tri7.uv0 = t0;
    tri7.uv1 = t1;
    tri7.uv2 = t2;
    tri7.color = Color(255, 255, 0);
    triangles.push_back(tri7);

    Triangle tri8;
    tri8.v0 = v0;
    tri8.v1 = v5;
    tri8.v2 = v4;
    tri8.uv0 = t0;
    tri8.uv1 = t2;
    tri8.uv2 = t3;
    tri8.color = Color(255, 255, 0);
    triangles.push_back(tri8);

    Triangle tri9;
    tri9.v0 = v1;
    tri9.v1 = v2;
    tri9.v2 = v6;
    tri9.uv0 = t0;
    tri9.uv1 = t1;
    tri9.uv2 = t2;
    tri9.color = Color(255, 0, 255);
    triangles.push_back(tri9);

    Triangle tri10;
    tri10.v0 = v1;
    tri10.v1 = v6;
    tri10.v2 = v5;
    tri10.uv0 = t0;
    tri10.uv1 = t2;
    tri10.uv2 = t3;
    tri10.color = Color(255, 0, 255);
    triangles.push_back(tri10);

    Triangle tri11;
    tri11.v0 = v0;
    tri11.v1 = v4;
    tri11.v2 = v7;
    tri11.uv0 = t0;
    tri11.uv1 = t1;
    tri11.uv2 = t2;
    tri11.color = Color(0, 255, 255);
    triangles.push_back(tri11);

    Triangle tri12;
    tri12.v0 = v0;
    tri12.v1 = v7;
    tri12.v2 = v3;
    tri12.uv0 = t0;
    tri12.uv1 = t2;
    tri12.uv2 = t3;
    tri12.color = Color(0, 255, 255);
    triangles.push_back(tri12);

    return triangles;
}

Matrix4 create_rotation_y(float angle)
{
    Matrix4 mat;
    int cos_val = static_cast<int>(std::cos(angle) * 4096);
    int sin_val = static_cast<int>(std::sin(angle) * 4096);

    mat.m[0] = cos_val;
    mat.m[2] = -sin_val;
    mat.m[8] = sin_val;
    mat.m[10] = cos_val;

    return mat;
}

Matrix4 create_rotation_x(float angle)
{
    Matrix4 mat;
    int cos_val = static_cast<int>(std::cos(angle) * 4096);
    int sin_val = static_cast<int>(std::sin(angle) * 4096);

    mat.m[5] = cos_val;
    mat.m[6] = -sin_val;
    mat.m[9] = sin_val;
    mat.m[10] = cos_val;

    return mat;
}

Matrix4 create_rotation_z(float angle)
{
    Matrix4 mat;
    int cos_val = static_cast<int>(std::cos(angle) * 4096);
    int sin_val = static_cast<int>(std::sin(angle) * 4096);

    mat.m[0] = cos_val;
    mat.m[1] = -sin_val;
    mat.m[4] = sin_val;
    mat.m[5] = cos_val;

    return mat;
}

Matrix4 create_translation(int x, int y, int z)
{
    Matrix4 mat;
    mat.m[3] = x;
    mat.m[7] = y;
    mat.m[11] = z;
    return mat;
}

Matrix4 create_view_matrix(float camera_distance)
{
    Matrix4 view;

    view.m[11] = -camera_distance * 4096;

    return view;
}

GLuint create_checkerboard_texture(int width, int height)
{
    unsigned char *data = new unsigned char[width * height * 3];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int idx = (y * width + x) * 3;
            bool is_white = ((x / 16) + (y / 16)) % 2 == 0;
            unsigned char color = is_white ? 255 : 64;

            data[idx] = color;
            data[idx + 1] = color;
            data[idx + 2] = color;
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    delete[] data;
    return texture;
}

int main()
{
    try
    {

        Renderer renderer(800, 600, "Cube");

        std::vector<Triangle> cube = create_cube(4096 * 4);

        GLuint texture = create_checkerboard_texture(128, 128);

        float rotation = 0.0f;
        float camera_distance = 0.7f;

        auto last_time = std::chrono::high_resolution_clock::now();

        while (!renderer.should_close())
        {

            auto current_time = std::chrono::high_resolution_clock::now();
            float delta_time = std::chrono::duration<float>(current_time - last_time).count();
            last_time = current_time;

            rotation += delta_time * 2.0f;

            Matrix4 view = create_view_matrix(camera_distance);

            Matrix4 rot_x = create_rotation_x(rotation * 0.7f);
            Matrix4 rot_y = create_rotation_y(rotation);
            Matrix4 rot_z = create_rotation_z(rotation * 1.3f);

            Matrix4 rotation_matrix = matrix_multiply(rot_z, matrix_multiply(rot_y, rot_x));

            renderer.set_view_matrix(view);

            renderer.begin_frame();

            std::vector<Triangle> transformed_cube;
            for (const auto &tri : cube)
            {
                Triangle t;
                t.v0 = matrix_multiply(rotation_matrix, tri.v0);
                t.v1 = matrix_multiply(rotation_matrix, tri.v1);
                t.v2 = matrix_multiply(rotation_matrix, tri.v2);
                t.uv0 = tri.uv0;
                t.uv1 = tri.uv1;
                t.uv2 = tri.uv2;
                t.color = tri.color;
                transformed_cube.push_back(t);
            }

            renderer.render(transformed_cube, texture);

            renderer.end_frame();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
