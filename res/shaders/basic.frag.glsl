#version 450 core

layout (location = 0) out vec4 fColor;

in float h;

void main() {
    if (h > 0) {
        fColor = vec4(h, 0.0, 0.0, 1.0);
    }
    else {
        fColor = vec4(0.0, -h, 0.0, 1.0);
    }
}
