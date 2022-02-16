//
// Created by pbialas on 05.08.2020.
// Modified by Mikolaj B on 14.02.2022
//

#pragma once

#include <vector>
#include "Application/application.h"
#include "Application/utils.h"
#include "camera.h"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "camera_controler.h"
#include <Engine/Mesh.h>


class SimpleShapeApplication : public xe::Application {
public:

    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title,
                                                                                               debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }

    Camera *camera() { return camera_; }

    void cursor_position_callback(double x, double y) {
        Application::cursor_position_callback(x, y);
        if (controler_) {
            controler_->mouse_moved(x, y);
        }
    }

    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0f);
    }

    void mouse_button_callback(int button, int action, int mods) {
        Application::mouse_button_callback(button, action, mods);

        if (controler_) {
            double x, y;
            glfwGetCursorPos(window_, &x, &y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                controler_->LMB_pressed(x, y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                controler_->LMB_released(x, y);
        }

    }
    void add_submesh(xe::Mesh* mesh) {
        meshes_.push_back(mesh);
    }

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void set_controler(CameraControler *controler) { controler_ = controler; }

private:
    GLuint vao_;
    GLuint pvm_buffer_handle;
    Camera *camera_;
    CameraControler *controler_;
    std::vector<xe::Mesh *> meshes_;
};