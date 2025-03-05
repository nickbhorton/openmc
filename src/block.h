#pragma once

#include <array>
#include <cstdint>

#include "face.h"

namespace block
{
uint32_t constexpr stone = 1;
uint32_t constexpr sand = 2;
uint32_t constexpr grass = 3;
uint32_t constexpr dirt = 4;
} // namespace block

class Block
{
    std::array<std::array<uint32_t, 2>, 6> face_textures;
    std::array<TextureRotation, 6> face_orientations;

public:
    Block(
        std::array<std::array<uint32_t, 2>, 6> face_textures,
        std::array<TextureRotation, 6> face_orientations
    );
    uint32_t get_face(std::array<uint32_t, 3> position, Direction d) const;
};
