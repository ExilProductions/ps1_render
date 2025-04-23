#include "mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

std::map<std::string, Color> load_mtl_colors(const std::string& mtl_path)
{
    std::map<std::string, Color> material_colors;
    std::ifstream file(mtl_path);
    if (!file.is_open()) {
        std::cerr << "Warning: could not open MTL file: " << mtl_path << std::endl;
        return material_colors;
    }

    std::string line, current_mtl;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "newmtl") {
            ss >> current_mtl;
        } else if (keyword == "Kd") {
            float r, g, b;
            ss >> r >> g >> b;
            material_colors[current_mtl] = Color(
                static_cast<uint8_t>(r * 255),
                static_cast<uint8_t>(g * 255),
                static_cast<uint8_t>(b * 255)
            );
        }
    }

    return material_colors;
}

std::vector<Triangle> load_obj(const std::string& obj_path)
{
    std::ifstream file(obj_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open OBJ file: " + obj_path);
    }

    std::vector<Vec3> positions;
    std::vector<Vec2> texcoords;
    std::map<std::string, Color> materials;
    std::vector<Triangle> triangles;

    std::string current_material = "";
    fs::path base_path = fs::path(obj_path).parent_path();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            positions.emplace_back((int)(x * 4096), (int)(y * 4096), (int)(z * 4096));
        } else if (keyword == "vt") {
            float u, v;
            ss >> u >> v;
            texcoords.emplace_back(u, 1.0f - v);  // flip v
        } else if (keyword == "f") {
            std::vector<std::string> tokens;
            std::string token;
            while (ss >> token) tokens.push_back(token);

            std::vector<int> v_idx, t_idx;
            for (const auto& t : tokens) {
                std::stringstream ts(t);
                std::string part;
                int vi = -1, ti = -1;

                std::getline(ts, part, '/');
                if (!part.empty()) vi = std::stoi(part) - 1;

                if (std::getline(ts, part, '/') && !part.empty())
                    ti = std::stoi(part) - 1;

                v_idx.push_back(vi);
                t_idx.push_back(ti);
            }

            // Triangulate face
            for (size_t i = 1; i + 1 < v_idx.size(); ++i) {
                Triangle tri;
                tri.v0 = positions[v_idx[0]];
                tri.v1 = positions[v_idx[i]];
                tri.v2 = positions[v_idx[i + 1]];

                tri.uv0 = (t_idx[0] >= 0 && t_idx[0] < (int)texcoords.size()) ? texcoords[t_idx[0]] : Vec2(0, 0);
                tri.uv1 = (t_idx[i] >= 0 && t_idx[i] < (int)texcoords.size()) ? texcoords[t_idx[i]] : Vec2(0, 0);
                tri.uv2 = (t_idx[i + 1] >= 0 && t_idx[i + 1] < (int)texcoords.size()) ? texcoords[t_idx[i + 1]] : Vec2(0, 0);

                if (materials.count(current_material)) {
                    tri.color = materials[current_material];
                } else {
                    tri.color = Color(255, 255, 255); // fallback white
                }

                triangles.push_back(tri);
            }
        } else if (keyword == "mtllib") {
            std::string mtl_file;
            ss >> mtl_file;
            fs::path mtl_path = base_path / mtl_file;
            materials = load_mtl_colors(mtl_path.string());
        } else if (keyword == "usemtl") {
            ss >> current_material;
        }
    }

    return triangles;
}
