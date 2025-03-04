#pragma once

#include <array>
#include <cstdint>

static constexpr uint32_t g_chunk_size{32};
static constexpr uint32_t blocks_in_chunk{
    g_chunk_size * g_chunk_size * g_chunk_size
};

struct Chunk {
    Chunk();
    std::array<uint64_t, blocks_in_chunk / 64> block_mask;
    bool test_block_mask(int32_t x, int32_t y, int32_t z);
    void set_block_mask(uint32_t x, uint32_t y, uint32_t z);
    void unset_block_mask(uint32_t x, uint32_t y, uint32_t z);
};
