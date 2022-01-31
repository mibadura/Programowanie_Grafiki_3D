//
// Created by pbialas on 25.09.2020.
// Modified by Mikolaj on 31.01.2022
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"

void SimpleShapeApplication::init()
{
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
        {{GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program)
    {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
        -0.5f, -0.5f, -0.5f,    1.0f, 0.38f, 0.48f, //back
        0.0f, 0.5f, 0.0f,       1.0f, 0.38f, 0.48f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.38f, 0.48f,

        -0.5f, -0.5f, 0.5f,     0.0f, 0.38f, 0.48f, //left pyramid side
        0.0f, 0.5f, 0.0f,       0.0f, 0.38f, 0.48f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.38f, 0.48f,

        0.5f, -0.5f, -0.5f,     0.40f, 0.43f, 0.07f,    //right pyramid side
        0.0f, 0.5f, 0.0f,       0.40f, 0.43f, 0.07f,
        0.5f, -0.5f, 0.5f,      0.40f, 0.43f, 0.07f,

        0.5f, -0.5f, 0.5f,      0.96f, 0.74f, 0.0f, //front pyramid side
        0.0f, 0.5f, 0.0f,       0.96f, 0.74f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.96f, 0.74f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.71f, 0.66f, 0.60f,    //bottom1
        0.5f, -0.5f, -0.5f,     0.71f, 0.66f, 0.60f,
        -0.5f, -0.5f, 0.5f,     0.71f, 0.66f, 0.60f,

        0.5f, -0.5f, 0.5f,      0.0f, 0.61f, 0.87f, //bottom2
        -0.5f, -0.5f, 0.5f,     0.0f, 0.61f, 0.87f,
        0.5f, -0.5f, -0.5f,     0.0f, 0.61f, 0.87f,
    };

    std::vector<GLushort> indices = {
       0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
    };

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generating the buffer and loading the index data into it.
    GLuint i_buffer_handle;
    glGenBuffers(1, &i_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    //creating the buffer
    GLuint uniform_buffer;
    glGenBuffers(1, &uniform_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);
    float strength = 1.0;   //strength used in fragment shader
    float color[3] = { 0.7, 0.7, 0.7 };   //color used in fragment shader
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 1 * sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), &color);

    //added from RESIZING readme
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    aspect_ = (float)w/h;
    fov_ = glm::pi<float>()/4.0;
    near_ = 0.1f;
    far_ = 100.0f;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
    V_ = glm::lookAt(glm::vec3{0.0f, 5.0f, 5.0f},glm::vec3{0.0f, 0.0f, 0.0f},glm::vec3{0.0f, 0.0f, -1.0f});
    glm::mat4 PVM = P_ * V_; //PVM matrix

    GLuint buffer_trans;
    glGenBuffers(1, &buffer_trans);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_trans);
    glBufferData(GL_UNIFORM_BUFFER, 40 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer_trans);


    float theta = 2.2 * glm::pi<float>() / 6.0f;    //rotation - looking from the top of the pyramid down
    auto cs = std::cos(theta);
    auto ss = std::sin(theta);
    glm::mat3 rot{ 1.0f, 0.0f, 0.0f, 0.0f, cs, ss, 0.0f, -ss, cs };

    glm::vec3 trans{ 0.0, 0.0, 0.0 };
    glm::vec3 scale{ 1.0, 1.0, 1.0 };


    glBufferSubData(GL_UNIFORM_BUFFER, 8 * sizeof(float), 3 * sizeof(float), &rot[0]);  //rotation
    glBufferSubData(GL_UNIFORM_BUFFER, 12 * sizeof(float), 3 * sizeof(float), &rot[1]); //rotation2
    glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 3 * sizeof(float), &rot[2]); //rptation3

    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(float), 3 * sizeof(float), &scale);   //scale
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), &trans);   //tanslation


    //vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))); // Colors

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao

    //background color
    glClearColor(0.91f, 0.48f, 0.60f, 1.0f);

    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glUseProgram(program);
}

void SimpleShapeApplication::frame()
{
    auto PVM = P_ * V_;
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {    //from Resizing README
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h); 
    aspect_ = (float) w / h;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
}
