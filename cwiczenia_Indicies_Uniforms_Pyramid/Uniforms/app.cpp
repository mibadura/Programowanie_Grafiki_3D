//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"

#include "glm/glm.hpp"  //necessary dependencies
#include "glm/gtc/constants.hpp"
#include "glm/gtx/string_cast.hpp"

void SimpleShapeApplication::init()
{
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
        { {GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

    if (!program)
    {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    // -- UNIFORMS -- //
    GLuint buffer_uniforms(0u); //creating the buffer
    glGenBuffers(1, &buffer_uniforms);

    glBindBuffer(GL_UNIFORM_BUFFER, buffer_uniforms);   //binding the buffer
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);    //loading the data to the buffer


    float strength = 0.9;   //strength used in fragment shader
    float color[3] = {0.9, 0.1, 0.3};   //color used in fragment shader

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color);



    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffer_uniforms);

    //--MOVING HOUSES--//
    GLuint moving(0);
    glGenBuffers(1, &moving);
    glBindBuffer(GL_UNIFORM_BUFFER, moving);
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, moving);
    
        //copied from instructions//
    float theta = 1.0 * glm::pi<float>() / 6.0f;
    auto cs = std::cos(theta);
    auto ss = std::sin(theta);
    glm::mat2 rot{ cs,ss,-ss,cs };
    glm::vec2 trans{ 0.0,  -0.25 };
    glm::vec2 scale{ 0.5, 0.5 };
        //---//

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &scale);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), sizeof(glm::vec2), &trans);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 4 * sizeof(float), &rot[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 8 * sizeof(float), 4 * sizeof(float), &rot[1]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // -- END OF UNIFORMS -- //

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
        -0.5f, 0.0f, 0.0f,0.9f, 0.5f, 0.3f,
        0.0f, 0.5f, 0.0f,0.9f, 0.5f, 0.2f,
        0.5f, 0.0f, 0.0f,0.9f, 0.5f, 0.8f,
        0.5f, -0.5f, 0.0f,0.1f, 0.4f, 0.6f,
        -0.5f, -0.5f, 0.0f,0.1f, 0.5f, 0.0f};

    std::vector<GLushort> indices = { 
        0, 1, 2,    //indices are in order, top triangle, right bottom, left bottom
        2, 3, 4,
        4, 0, 2};   // creating the Glushort vector to store indexes off all distinct verticies

    GLuint buffers; //according to documentation: "specifies the array in which the generated buffer object names are stored"
    glGenBuffers(1, &buffers);  //void glGenBuffers(GLsizei n, GLuint* buffers); where n = Specifies the number of buffer object names to be generated.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers); //void glBindBuffer(	GLenum target, GLuint buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);   //very similar to the lines below, but using other target and type od data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   //unbinding the buffer



     //Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle; 
    glGenBuffers(1, &v_buffer_handle);  
    //   OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));

    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle); 

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);    
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GL_FLOAT)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame()
{
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, NULL);   // changed to draw from elements not array
    glBindVertexArray(0);
}
