//
// Created by pbialas on 25.09.2020.
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
        { {GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

    if (!program)
    {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

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
