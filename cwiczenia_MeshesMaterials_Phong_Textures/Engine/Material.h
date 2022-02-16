//
// Created by Piotr Białas on 02/11/2021.
//Modified by Mikolaj B on 14.02.2022

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"

namespace xe {

    class Material {
    public:
        virtual void bind() = 0;

        virtual void unbind() {};
    };
}