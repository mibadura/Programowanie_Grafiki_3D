#version 420

layout(location=0) out vec4 vFragColor;
in vec3 v_color;

layout(std140, binding = 0) uniform Modifier {
 float strength;
 vec3  color; 
};

void main() {
    vFragColor = vec4(v_color[0], v_color[1], v_color[2], 1);
    vFragColor.rgb = strength*color*v_color.rgb;
}
