#pragma once

#include <array>
#include <cstdint>
#include <vector>

static constexpr uint32_t g_chunk_size{32};
static constexpr uint32_t blocks_in_chunk{
    g_chunk_size * g_chunk_size * g_chunk_size
};

class Chunk
{
    std::array<uint8_t, blocks_in_chunk> block_mask;
    uint32_t test_block_mask(int32_t x, int32_t y, int32_t z);

public:
    Chunk();
    void set_block(uint32_t x, uint32_t y, uint32_t z, uint32_t block);
    void unset_block(uint32_t x, uint32_t y, uint32_t z);
    std::vector<uint32_t> mesh();
};
