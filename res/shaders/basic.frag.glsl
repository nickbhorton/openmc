#version 450 core

layout (location = 0) out vec4 fColor;

in vec2 f_uv;

uniform sampler2D tex;

void main() {
    vec4 texture_out = texture(tex, f_uv);
    fColor = vec4(texture_out.rgb, 1.0);
}
