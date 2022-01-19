#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec3 a_vertex_color;
out vec3 v_Color;

layout(std140, binding=1) uniform Transformations {
 vec3 scale;
 vec3 translation;
 mat3 rotation;
};

void main() {
    gl_Position = a_vertex_position;
    v_Color = a_vertex_color;

    //gl_Position.xy = rotation*(scale*a_vertex_position.xy)+translation;
    gl_Position.xyz = rotation*(scale*a_vertex_position.xyz)+translation;
    //gl_Position.zw = a_vertex_position.zw;
}
