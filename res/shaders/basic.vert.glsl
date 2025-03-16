#version 450 core

in vec3 v_position;
in vec3 v_color;

out vec3 f_color;

uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * vec4(v_position, 1.0);
    f_color = v_color;
}
