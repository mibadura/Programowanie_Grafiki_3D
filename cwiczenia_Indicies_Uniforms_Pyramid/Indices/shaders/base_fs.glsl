#version 410

layout(location=0) out vec4 vFragColor;
in vec3 v_color;

void main() {
    vFragColor = vec4(v_color[0], v_color[1], v_color[2], 1);
}
