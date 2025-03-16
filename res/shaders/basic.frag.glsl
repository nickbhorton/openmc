#version 450 core

layout (location = 0) out vec4 fColor;

in vec3 f_color;

void main() {
    fColor = vec4(f_color, 1.0);
}
