#pragma once

#include <array>
#include <cstdint>

#include "face.h"

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
