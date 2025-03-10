#version 450 core

in vec4 v_position;
in vec2 v_uv;
in uint v_offset;

out vec2 f_uv;
out float dim;

uniform mat4 view;
uniform mat4 proj;

uniform vec2 chunk_offset;

void main() {
    uint x_off = v_offset & 31;
    uint y_off = (v_offset >> 5 ) & 31;
    uint z_off = (v_offset >> 10) & 31;

    vec3 offset = vec3(float(x_off), float(y_off), float(z_off));
    vec4 base_pos = v_position;

    dim = 1.0;
    /*
    0: down
    1: up
    2: north
    3: east
    4: south
    5: west
    */
    uint direction = (v_offset >> 15) & 7;
    if (direction == 1) {
        base_pos.y = 1.0 - base_pos.y;
        base_pos.x = 1.0 - base_pos.x;
    }
    else if (direction == 2) {
        base_pos.y = base_pos.y + 1.0;
        base_pos.yz = base_pos.zy;
        dim = 0.86;
    }
    else if (direction == 3) {
        base_pos.x = 1.0 - base_pos.x;
        base_pos.xy = base_pos.yx;
        dim = 0.8;
    }
    else if (direction == 4) {
        base_pos.z = 1.0 - base_pos.z;
        base_pos.yz = base_pos.zy;
        dim = 0.86;
    }
    else if (direction == 5) {
        base_pos.y = base_pos.y + 1.0;
        base_pos.xy = base_pos.yx;
        dim = 0.8;
    }

    gl_Position = proj * view * (vec4(chunk_offset.x, 0.0, chunk_offset.y, 0.0) + base_pos + vec4(offset, 0.0));

    vec2 uv_array[16];
    uint tex_rotation = (v_offset >> 18) & 3;
    uv_array[0] = vec2(0.0,0.0);
    uv_array[1] = vec2(1.0,0.0);
    uv_array[2] = vec2(0.0,1.0);
    uv_array[3] = vec2(1.0,1.0);

    uv_array[4] = vec2(1.0,0.0);
    uv_array[5] = vec2(1.0,1.0);
    uv_array[6] = vec2(0.0,0.0);
    uv_array[7] = vec2(0.0,1.0);

    uv_array[8] = vec2(1.0,1.0);
    uv_array[9] = vec2(0.0,1.0);
    uv_array[10] = vec2(1.0,0.0);
    uv_array[11] = vec2(0.0,0.0);

    uv_array[12] = vec2(0.0,1.0);
    uv_array[13] = vec2(0.0,0.0);
    uv_array[14] = vec2(1.0,1.0);
    uv_array[15] = vec2(1.0,0.0);

    uint texture_index_x = (v_offset >> 20) & 31;
    uint texture_index_y = (v_offset >> 25) & 31;
    uint texture_discritization = 32;

    f_uv = uv_array[(4 * tex_rotation) + gl_VertexID] 
        * (1.0 / float(texture_discritization))
        + (1.0 / float(texture_discritization))
        * vec2(float(texture_index_x), float(texture_index_y));
}
