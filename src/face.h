#pragma once

#include <cstdint>
#include <glm/ext/vector_uint2.hpp>
#include <glm/ext/vector_uint3.hpp>

enum Direction {
    Down = 0,
    Up = 1,
    North = 2,
    East = 3,
    South = 4,
    West = 5,
};

enum TextureRotation {
    None = 0,
    CW90 = 1,
    CW180 = 2,
    CW270 = 3,
};

uint32_t generate_face_info(
    glm::uvec3 position,
    glm::uvec2 texture,
    Direction dir,
    TextureRotation rot
);
