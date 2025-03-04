#version 450 core

in vec4 v_position;
in vec2 v_uv;
in uint v_offset;

out vec2 f_uv;

uniform mat4 view;
uniform mat4 proj;

void main() {
    uint x_off = v_offset & 63;
    uint y_off = (v_offset >> 6 ) & 63;
    uint z_off = (v_offset >> 12) & 63;

    vec3 offset = vec3(float(x_off), float(y_off), float(z_off));
    vec4 base_pos = v_position + vec4(offset, 0.0);

    /*
    0: down
    1: up
    2: north
    3: east
    4: south
    5: west
    */
    uint rot = (v_offset >> 18) & 7;
    if (rot == 1) {
        base_pos.y + 1;
    }
    else if (rot == 3) {
        base_pos.xy = base_pos.yx;
    }
    gl_Position = proj * view * base_pos;
    f_uv = v_uv;
}
