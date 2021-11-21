#version 410

layout(location=0) out vec4 vFragColor;
in vec3 vec_out_colors;

void main() {
    vFragColor = vec4(vec_out_colors[0], vec_out_colors[1], vec_out_colors[2], 1);
}
