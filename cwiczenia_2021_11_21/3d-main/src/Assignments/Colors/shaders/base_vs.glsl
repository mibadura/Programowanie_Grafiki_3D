#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec3 vec_in_colors;
out vec3 vec_out_colors;


void main() {
    gl_Position = a_vertex_position;
	vec_out_colors = vec_in_colors;
}
