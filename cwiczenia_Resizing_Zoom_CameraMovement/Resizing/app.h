//
// Created by pbialas on 05.08.2020.
// Modified by Mikolaj on 31.01.2022
//

#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/gl.h"
#include "Application/application.h"
#include "Application/utils.h"


class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;
    void framebuffer_resize_callback(int w, int h) override; 

private:
    GLuint vao_;
    std::vector<GLushort> indices;
    float fov_; //copied from README
    float aspect_;
    float near_;
    float far_; 
    glm::mat4 P_;
    glm::mat4 V_; 
    GLuint u_pvm_buffer_;
};