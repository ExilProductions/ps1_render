#pragma once

#include <string>
#include <vector>
#include <map>
#include "renderer.h" // includes Vec3, Vec2, Triangle, Color

std::vector<Triangle> load_obj(const std::string& obj_path);
std::map<std::string, Color> load_mtl_colors(const std::string& mtl_path);
