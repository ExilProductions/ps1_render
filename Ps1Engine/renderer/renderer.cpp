#include "renderer.h"
#include <stdexcept>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define FIXED_POINT_PRECISION 4096
#define FIXED_POINT_SHIFT 12

Vec3::Vec3(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}

Vec2::Vec2(float x_, float y_) : x(x_), y(y_) {}

Color::Color(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}

Matrix4::Matrix4()
{
    for (int i = 0; i < 16; i++)
        m[i] = (i % 5 == 0) ? FIXED_POINT_PRECISION : 0;
}

Vec3 matrix_multiply(const Matrix4 &mat, const Vec3 &v)
{
    int x = (mat.m[0] * v.x + mat.m[1] * v.y + mat.m[2] * v.z + mat.m[3] * FIXED_POINT_PRECISION) / FIXED_POINT_PRECISION;
    int y = (mat.m[4] * v.x + mat.m[5] * v.y + mat.m[6] * v.z + mat.m[7] * FIXED_POINT_PRECISION) / FIXED_POINT_PRECISION;
    int z = (mat.m[8] * v.x + mat.m[9] * v.y + mat.m[10] * v.z + mat.m[11] * FIXED_POINT_PRECISION) / FIXED_POINT_PRECISION;
    return Vec3(x, y, z);
}

Matrix4 matrix_multiply(const Matrix4 &a, const Matrix4 &b)
{
    Matrix4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result.m[row * 4 + col] =
                (a.m[row * 4 + 0] * b.m[col + 0]) +
                (a.m[row * 4 + 1] * b.m[col + 4]) +
                (a.m[row * 4 + 2] * b.m[col + 8]) +
                (a.m[row * 4 + 3] * b.m[col + 12]);

            result.m[row * 4 + col] /= 4096;
        }
    }

    return result;
}

Color convert_color(const Color &color)
{

    uint8_t r = (color.r >> 3) << 3;
    uint8_t g = (color.g >> 3) << 3;
    uint8_t b = (color.b >> 3) << 3;
    return Color(r, g, b);
}

Renderer::Renderer(int width_, int height_, const std::string &title_)
    : width(width_), height(height_), title(title_),
      internal_width(320), internal_height(240)
{

    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -32768, 32767);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
}

Renderer::~Renderer()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Renderer::set_view_matrix(const Matrix4 &view)
{
    view_matrix = view;
}

void Renderer::begin_frame()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const std::vector<Triangle> &triangles, GLuint texture)
{
    std::vector<Triangle> transformed_triangles;

    for (const auto &tri : triangles)
    {
        Triangle t;
        t.v0 = matrix_multiply(view_matrix, tri.v0);
        t.v1 = matrix_multiply(view_matrix, tri.v1);
        t.v2 = matrix_multiply(view_matrix, tri.v2);
        t.uv0 = tri.uv0;
        t.uv1 = tri.uv1;
        t.uv2 = tri.uv2;
        t.color = tri.color;

        Vec3 edge1(t.v1.x - t.v0.x, t.v1.y - t.v0.y, t.v1.z - t.v0.z);
        Vec3 edge2(t.v2.x - t.v0.x, t.v2.y - t.v0.y, t.v2.z - t.v0.z);
        Vec3 normal(
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x);

        if (normal.z > 0)
        {
            transformed_triangles.push_back(t);
        }
    }

    std::sort(transformed_triangles.begin(), transformed_triangles.end(),
              [](const Triangle &a, const Triangle &b)
              {
                  int z_a = (a.v0.z + a.v1.z + a.v2.z) / 3;
                  int z_b = (b.v0.z + b.v1.z + b.v2.z) / 3;
                  return z_a > z_b;
              });

    std::vector<Triangle> screen_triangles;
    for (const auto &tri : transformed_triangles)
    {
        Triangle screen_tri;

        float scale_x = (float)width / internal_width;
        float scale_y = (float)height / internal_height;

        screen_tri.v0 = Vec3(
            ((tri.v0.x * internal_width / 32) / FIXED_POINT_PRECISION) * scale_x + width / 2,
            ((tri.v0.y * internal_height / 32) / FIXED_POINT_PRECISION) * scale_y + height / 2,
            tri.v0.z);
        screen_tri.v1 = Vec3(
            ((tri.v1.x * internal_width / 32) / FIXED_POINT_PRECISION) * scale_x + width / 2,
            ((tri.v1.y * internal_height / 32) / FIXED_POINT_PRECISION) * scale_y + height / 2,
            tri.v1.z);
        screen_tri.v2 = Vec3(
            ((tri.v2.x * internal_width / 32) / FIXED_POINT_PRECISION) * scale_x + width / 2,
            ((tri.v2.y * internal_height / 32) / FIXED_POINT_PRECISION) * scale_y + height / 2,
            tri.v2.z);

        int snap_factor = 4;
        screen_tri.v0.x = (screen_tri.v0.x / snap_factor) * snap_factor;
        screen_tri.v0.y = (screen_tri.v0.y / snap_factor) * snap_factor;
        screen_tri.v1.x = (screen_tri.v1.x / snap_factor) * snap_factor;
        screen_tri.v1.y = (screen_tri.v1.y / snap_factor) * snap_factor;
        screen_tri.v2.x = (screen_tri.v2.x / snap_factor) * snap_factor;
        screen_tri.v2.y = (screen_tri.v2.y / snap_factor) * snap_factor;

        if (rand() % 10 == 0)
        {
            screen_tri.v0.x += (rand() % 3) - 1;
            screen_tri.v0.y += (rand() % 3) - 1;
        }

        screen_tri.uv0 = tri.uv0;
        screen_tri.uv1 = tri.uv1;
        screen_tri.uv2 = tri.uv2;
        screen_tri.color = convert_color(tri.color);

        screen_triangles.push_back(screen_tri);
    }

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDisable(GL_BLEND);

    for (const auto &tri : screen_triangles)
    {

        glBegin(GL_TRIANGLES);

        Color c = tri.color;
        glColor3ub(c.r, c.g, c.b);

        glTexCoord2f(tri.uv0.x, tri.uv0.y);
        glVertex3f(static_cast<float>(tri.v0.x), static_cast<float>(tri.v0.y), static_cast<float>(tri.v0.z) / 32767.0f);

        glTexCoord2f(tri.uv1.x, tri.uv1.y);
        glVertex3f(static_cast<float>(tri.v1.x), static_cast<float>(tri.v1.y), static_cast<float>(tri.v1.z) / 32767.0f);

        glTexCoord2f(tri.uv2.x, tri.uv2.y);
        glVertex3f(static_cast<float>(tri.v2.x), static_cast<float>(tri.v2.y), static_cast<float>(tri.v2.z) / 32767.0f);

        glEnd();

        if (rand() % 5 == 0)
        {
            glDisable(GL_TEXTURE_2D);
            glColor3f(0.0f, 0.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            glVertex3f(static_cast<float>(tri.v0.x), static_cast<float>(tri.v0.y), static_cast<float>(tri.v0.z) / 32767.0f);
            glVertex3f(static_cast<float>(tri.v1.x), static_cast<float>(tri.v1.y), static_cast<float>(tri.v1.z) / 32767.0f);
            glVertex3f(static_cast<float>(tri.v2.x), static_cast<float>(tri.v2.y), static_cast<float>(tri.v2.z) / 32767.0f);
            glEnd();
            glEnable(GL_TEXTURE_2D);
        }
    }
}

void Renderer::end_frame()
{

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Renderer::should_close()
{
    return glfwWindowShouldClose(window);
}

GLuint Renderer::load_texture(const std::string &filepath)
{
    int width, height, channels;
    unsigned char *image = stbi_load(filepath.c_str(), &width, &height, &channels, 3);
    if (!image)
    {
        throw std::runtime_error("Failed to load texture: " + filepath);
    }

    int new_width = std::min(width, 256);
    int new_height = std::min(height, 256);

    if (new_width > 128)
        new_width = 256;
    else if (new_width > 64)
        new_width = 128;
    else if (new_width > 32)
        new_width = 64;
    else if (new_width > 16)
        new_width = 32;
    else
        new_width = 16;

    if (new_height > 128)
        new_height = 256;
    else if (new_height > 64)
        new_height = 128;
    else if (new_height > 32)
        new_height = 64;
    else if (new_height > 16)
        new_height = 32;
    else
        new_height = 16;

    unsigned char *resized = new unsigned char[new_width * new_height * 3];

    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            int src_x = x * width / new_width;
            int src_y = y * height / new_height;
            int src_idx = (src_y * width + src_x) * 3;
            int dst_idx = (y * new_width + x) * 3;

            resized[dst_idx] = (image[src_idx] >> 3) << 3;
            resized[dst_idx + 1] = (image[src_idx + 1] >> 3) << 3;
            resized[dst_idx + 2] = (image[src_idx + 2] >> 3) << 3;
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, new_width, new_height, 0, GL_RGB, GL_UNSIGNED_BYTE, resized);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(image);
    delete[] resized;

    return texture;
}
