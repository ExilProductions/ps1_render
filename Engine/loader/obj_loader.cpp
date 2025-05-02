#include "obj_loader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <filesystem>

std::vector<Triangle> ObjLoader::load_obj(const std::string &obj_filepath,
                                          Renderer &renderer)
{
    std::vector<Triangle> triangles;
    std::vector<Vec3> vertices;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;
    std::map<std::string, Texture> material_textures;
    std::string current_material;
    std::string mtl_filepath;

    auto parse_mtl = [&](const std::string &mtl_path)
    {
        std::ifstream mtl_file(mtl_path);
        if (!mtl_file.is_open())
        {

            return;
        }

        std::string line, current_mtl;
        while (std::getline(mtl_file, line))
        {
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "newmtl")
            {
                iss >> current_mtl;
            }
            else if (token == "map_Kd")
            {
                std::string texture_path;
                std::getline(iss, texture_path);
                texture_path.erase(0, texture_path.find_first_not_of(" \t"));

                std::filesystem::path mtl_dir = std::filesystem::path(mtl_path).parent_path();
                std::filesystem::path tex_path = mtl_dir / texture_path;

                try
                {
                    material_textures[current_mtl] = renderer.load_texture(tex_path.string());
                }
                catch (const std::exception &e)
                {

                    material_textures[current_mtl] = Texture(0, 0, 0);
                }
            }
        }
        mtl_file.close();
    };

    std::ifstream obj_file(obj_filepath);
    if (!obj_file.is_open())
    {
        throw std::runtime_error("Failed to open OBJ file: " + obj_filepath);
    }

    std::filesystem::path obj_dir = std::filesystem::path(obj_filepath).parent_path();

    std::string line;
    while (std::getline(obj_file, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        else if (token == "vt")
        {
            float u, v;
            iss >> u >> v;
            uvs.emplace_back(u, v);
        }
        else if (token == "vn")
        {
            float x, y, z;
            iss >> x >> y >> z;
            normals.emplace_back(x, y, z);
        }
        else if (token == "f")
        {
            std::vector<std::tuple<int, int, int>> face_vertices;
            std::string vertex;
            while (iss >> vertex)
            {
                int v_idx = 0, vt_idx = 0, vn_idx = 0;
                std::sscanf(vertex.c_str(), "%d/%d/%d", &v_idx, &vt_idx, &vn_idx);
                face_vertices.emplace_back(v_idx - 1, vt_idx - 1, vn_idx - 1);
            }

            for (size_t i = 1; i < face_vertices.size() - 1; ++i)
            {
                Triangle tri;
                auto [v0, vt0, vn0] = face_vertices[0];
                auto [v1, vt1, vn1] = face_vertices[i];
                auto [v2, vt2, vn2] = face_vertices[i + 1];

                tri.v0 = vertices[v0];
                tri.v1 = vertices[v1];
                tri.v2 = vertices[v2];

                tri.uv0 = vt0 >= 0 && vt0 < uvs.size() ? uvs[vt0] : Vec2(0, 0);
                tri.uv1 = vt1 >= 0 && vt1 < uvs.size() ? uvs[vt1] : Vec2(0, 0);
                tri.uv2 = vt2 >= 0 && vt2 < uvs.size() ? uvs[vt2] : Vec2(0, 0);

                tri.color = Color(255, 255, 255);

                tri.texture = material_textures.count(current_material) ? material_textures[current_material] : Texture(0, 0, 0);

                triangles.push_back(tri);
            }
        }
        else if (token == "mtllib")
        {
            std::string mtl_name;
            iss >> mtl_name;
            mtl_filepath = (obj_dir / mtl_name).string();
            parse_mtl(mtl_filepath);
        }
        else if (token == "usemtl")
        {
            iss >> current_material;
        }
    }

    obj_file.close();
    return triangles;
}