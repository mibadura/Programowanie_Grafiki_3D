//
// Created by Piotr Białas on 04/11/2021.
//Modified by Mikolaj B on 14.02.2022



#pragma once

#include <string>
#include <memory>

namespace xe {
    class Mesh;

    Mesh *load_mesh_from_obj(std::string path, std::string mtl_dir);
}