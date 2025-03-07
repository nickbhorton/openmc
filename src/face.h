#pragma once

#include <array>
#include <cstdint>

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
    std::array<uint32_t, 3> position,
    std::array<uint32_t, 2> texture,
    Direction dir,
    TextureRotation rot
);
