#version 450 core

in vec4 v_position;
in vec2 v_uv;
in vec3 v_offset;

out vec2 f_uv;

uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * (v_position + vec4(2.0 * v_offset, 0.0));
    f_uv = v_uv;
}
