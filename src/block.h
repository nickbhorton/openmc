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
uint32_t constexpr dirt = 2;
uint32_t constexpr sand = 3;
uint32_t constexpr gravel = 4;
uint32_t constexpr bedrock = 5;
uint32_t constexpr cobblestone = 6;
uint32_t constexpr oak_log_UD = 6;
uint32_t constexpr oak_log_NS = 6;
uint32_t constexpr oak_log_EW = 6;
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
std::array<TextureRotation, 6> constexpr log_UD{
    {TextureRotation::CW180,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::CW270,
     TextureRotation::CW180,
     TextureRotation::CW90}
};
std::array<TextureRotation, 6> constexpr log_NS{
    {TextureRotation::CW180,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::CW180,
     TextureRotation::CW180}
};
std::array<TextureRotation, 6> constexpr log_EW{
    {TextureRotation::CW270,
     TextureRotation::CW90,
     TextureRotation::CW90,
     TextureRotation::CW270,
     TextureRotation::CW270,
     TextureRotation::CW90}
};
} // namespace trt

std::vector<Block> const g_blocks{
    Block(block::stone, 0, trt::homogeneous),
    Block(block::dirt, 1, trt::homogeneous),
    Block(block::sand, 2, trt::homogeneous),
    Block(block::gravel, 3, trt::homogeneous),
    Block(block::bedrock, 4, trt::homogeneous),
    Block(block::cobblestone, 5, trt::homogeneous),
    Block(block::oak_log_UD, {7, 7, 6, 6, 6, 6}, trt::log_UD),
    Block(block::oak_log_NS, {6, 6, 7, 6, 7, 6}, trt::log_NS),
    Block(block::oak_log_EW, {6, 6, 6, 7, 6, 7}, trt::log_EW),
};
