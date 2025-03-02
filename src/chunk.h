#pragma once

#include <array>
#include <cstdint>

static constexpr std::array<uint32_t, 3> g_chunk_size{{16, 256, 16}};
static constexpr uint32_t blocks_in_chunk{
    g_chunk_size[0] * g_chunk_size[1] * g_chunk_size[2]
};

struct Chunk {
    Chunk();
    std::array<uint64_t, blocks_in_chunk / 64> block_mask;
    bool test_block_mask(uint32_t x, uint32_t y, uint32_t z);
    void set_block_mask(uint32_t x, uint32_t y, uint32_t z);
};
