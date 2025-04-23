#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <cstdint>

struct Vec3
{
    int x, y, z;
    Vec3(int x_ = 0, int y_ = 0, int z_ = 0);
};

struct Vec2
{
    float x, y;
    Vec2(float x_ = 0.0f, float y_ = 0.0f);
};

struct Color
{
    uint8_t r, g, b;
    Color(uint8_t r_ = 255, uint8_t g_ = 255, uint8_t b_ = 255);
};

struct Matrix4
{
    int m[16];
    Matrix4();
};

struct Triangle
{
    Vec3 v0, v1, v2;
    Vec2 uv0, uv1, uv2;
    Color color;
    GLuint texture;
};

Vec3 matrix_multiply(const Matrix4 &mat, const Vec3 &v);

Matrix4 matrix_multiply(const Matrix4 &a, const Matrix4 &b);

Color convert_color(const Color &color);

class Renderer
{
private:
    GLFWwindow *window;
    Matrix4 view_matrix;
    int width;
    int height;
    int internal_width;
    int internal_height;
    std::string title;

public:
    Renderer(int width_, int height_, const std::string &title_);
    ~Renderer();
    void set_view_matrix(const Matrix4 &view);
    void begin_frame();
    void render(const std::vector<Triangle> &triangles);
    void end_frame();
    bool should_close();
    GLuint load_texture(const std::string &filepath);
    GLFWwindow *get_window() { return window; }
};

#endif