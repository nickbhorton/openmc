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
uint32_t constexpr oak_log_UD = 7;
uint32_t constexpr oak_log_NS = 8;
uint32_t constexpr oak_log_EW = 9;
uint32_t constexpr iron_block = 10;
uint32_t constexpr gold_block = 11;
uint32_t constexpr diamond_block = 12;
uint32_t constexpr black_wool = 13;
uint32_t constexpr blue_wool = 14;
uint32_t constexpr brown_wool = 15;
uint32_t constexpr cyan_wool = 16;
uint32_t constexpr gray_wool = 17;
uint32_t constexpr green_wool = 18;
uint32_t constexpr light_blue_wool = 19;
uint32_t constexpr light_gray_wool = 20;
uint32_t constexpr lime_wool = 21;
uint32_t constexpr magenta_wool = 22;
uint32_t constexpr orange_wool = 23;
uint32_t constexpr pink_wool = 24;
uint32_t constexpr purple_wool = 25;
uint32_t constexpr red_wool = 26;
uint32_t constexpr white_wool = 27;
uint32_t constexpr yellow_wool = 28;
uint32_t constexpr andesite = 29;
uint32_t constexpr diorite = 30;
uint32_t constexpr granite = 31;
uint32_t constexpr polished_andesite = 32;
uint32_t constexpr polished_diorite = 33;
uint32_t constexpr polished_granite = 34;
uint32_t constexpr bricks = 35;
uint32_t constexpr calcite = 36;
uint32_t constexpr chiseled_stone_bricks = 37;
uint32_t constexpr coarse_dirt = 38;
uint32_t constexpr deepslate = 39;
uint32_t constexpr cobbled_deepslate = 40;
uint32_t constexpr cracked_stone_bricks = 41;
uint32_t constexpr stone_bricks = 42;
uint32_t constexpr sandstone = 43;
uint32_t constexpr dark_prismarine = 44;
uint32_t constexpr red_sand = 45;
uint32_t constexpr red_sandstone = 46;
uint32_t constexpr quartz_bricks = 47;
uint32_t constexpr tuff = 48;
uint32_t constexpr packed_mud = 49;
uint32_t constexpr obsidian = 50;
uint32_t constexpr nether_bricks = 51;
uint32_t constexpr mud_bricks = 52;
uint32_t constexpr mud = 53;
uint32_t constexpr mossy_cobblestone = 54;
uint32_t constexpr mossy_stone_bricks = 55;
uint32_t constexpr moss_block = 56;
uint32_t constexpr oak_planks = 57;
uint32_t constexpr soul_san = 58;
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
    Block(block::iron_block, 8, trt::homogeneous),
    Block(block::gold_block, 9, trt::homogeneous),
    Block(block::diamond_block, 10, trt::homogeneous),
    Block(block::black_wool, 11, trt::homogeneous),
    Block(block::blue_wool, 12, trt::homogeneous),
    Block(block::brown_wool, 13, trt::homogeneous),
    Block(block::cyan_wool, 14, trt::homogeneous),
    Block(block::gray_wool, 15, trt::homogeneous),
    Block(block::green_wool, 16, trt::homogeneous),
    Block(block::light_blue_wool, 17, trt::homogeneous),
    Block(block::light_gray_wool, 18, trt::homogeneous),
    Block(block::lime_wool, 19, trt::homogeneous),
    Block(block::magenta_wool, 20, trt::homogeneous),
    Block(block::orange_wool, 21, trt::homogeneous),
    Block(block::pink_wool, 22, trt::homogeneous),
    Block(block::purple_wool, 23, trt::homogeneous),
    Block(block::red_wool, 24, trt::homogeneous),
    Block(block::white_wool, 25, trt::homogeneous),
    Block(block::yellow_wool, 26, trt::homogeneous),
    Block(block::andesite, 27, trt::homogeneous),
    Block(block::diorite, 28, trt::homogeneous),
    Block(block::granite, 29, trt::homogeneous),
    Block(block::polished_andesite, 30, trt::homogeneous),
    Block(block::polished_diorite, 31, trt::homogeneous),
    Block(block::polished_granite, 32, trt::homogeneous),
    Block(block::bricks, 33, trt::homogeneous),
    Block(block::calcite, 34, trt::homogeneous),
    Block(block::chiseled_stone_bricks, 35, trt::homogeneous),
    Block(block::coarse_dirt, 36, trt::homogeneous),
    Block(block::deepslate, 37, trt::homogeneous),
    Block(block::cobbled_deepslate, 38, trt::homogeneous),
    Block(block::cracked_stone_bricks, 39, trt::homogeneous),
    Block(block::stone_bricks, 40, trt::homogeneous),
    Block(block::sandstone, 41, trt::homogeneous),
    Block(block::dark_prismarine, 42, trt::homogeneous),
    Block(block::red_sand, 43, trt::homogeneous),
    Block(block::red_sandstone, 44, trt::homogeneous),
    Block(block::quartz_bricks, 45, trt::homogeneous),
    Block(block::tuff, 46, trt::homogeneous),
    Block(block::packed_mud, 47, trt::homogeneous),
    Block(block::obsidian, 48, trt::homogeneous),
    Block(block::nether_bricks, 49, trt::homogeneous),
    Block(block::mud_bricks, 50, trt::homogeneous),
    Block(block::mud, 51, trt::homogeneous),
    Block(block::mossy_cobblestone, 52, trt::homogeneous),
    Block(block::mossy_stone_bricks, 53, trt::homogeneous),
    Block(block::moss_block, 54, trt::homogeneous),
    Block(block::oak_planks, 55, trt::homogeneous),
    Block(block::soul_san, 56, trt::homogeneous),
};
