#version 450 core

in vec4 v_position;

uniform mat4 view;
uniform mat4 proj;

out float h;

void main() {
    gl_Position = proj * view * v_position;
    h = v_position.y;
}
