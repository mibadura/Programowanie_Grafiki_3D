//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/gl.h"
#include "Application/application.h"
#include "Application/utils.h"
#include "camera.h" //including the created file


class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;
    void framebuffer_resize_callback(int w, int h) override; 
    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    void scroll_callback(double xoffset, double yoffset) override;  //function for handling the scroll
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }
    

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

    Camera *camera_;    //added from ZOOM README
        
};