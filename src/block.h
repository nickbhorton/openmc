#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "face.h"

constexpr uint32_t g_texture_square_width{32};

class Block
{
    std::array<uint32_t, 6> face_textures;
    std::array<TextureRotation, 6> face_orientations;
    uint32_t id;

public:
    Block(
        uint32_t id,
        std::array<uint32_t, 6> face_textures,
        std::array<TextureRotation, 6> face_orientations
    );
    Block(
        uint32_t id,
        uint32_t face_texture,
        std::array<TextureRotation, 6> face_orientations
    );
    uint32_t get_face(std::array<uint32_t, 3> position, Direction d) const;

    uint32_t get_id() const;
};

namespace block
{
uint32_t constexpr stone = 1;
} // namespace block

// texture roatation types
namespace trt
{
std::array<TextureRotation, 6> constexpr homogeneous{
    {TextureRotation::CW180,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::CW270,
     TextureRotation::CW180,
     TextureRotation::CW90}
};
} // namespace trt

std::vector<Block> const g_blocks{Block(block::stone, 0, trt::homogeneous)};
