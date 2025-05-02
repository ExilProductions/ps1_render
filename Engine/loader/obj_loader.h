#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "renderer.h"
#include <string>
#include <vector>

class ObjLoader
{
public:
    static std::vector<Triangle> load_obj(const std::string& obj_filepath, 
                                        Renderer& renderer);
};

#endif