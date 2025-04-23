#pragma once

#include <string>
#include <vector>
#include <map>
#include "renderer.h" // includes Vec3, Vec2, Triangle, Color

struct Material
{
    Color color;
    std::string texture_path;
};

std::map<std::string, Material> load_mtl_materials(const std::string& mtl_path);
std::vector<Triangle> load_obj(const std::string& obj_path, Renderer* renderer);