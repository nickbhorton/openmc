#pragma once

#include <array>
#include <cstdint>
#include <vector>

static constexpr int32_t g_chunk_size{32};
static constexpr int32_t blocks_in_chunk{
    g_chunk_size * g_chunk_size * g_chunk_size
};

class Chunk
{
public:
    Chunk();
    void set_block(uint32_t x, uint32_t y, uint32_t z, uint32_t block);
    void unset_block(uint32_t x, uint32_t y, uint32_t z);
    uint32_t test_block_mask(int32_t x, int32_t y, int32_t z);
    uint32_t test_block_mask(std::array<int32_t, 3> at);
    uint32_t test_block_mask(std::array<int64_t, 3> at);
    std::vector<uint32_t> mesh();

private:
    std::array<uint8_t, blocks_in_chunk> block_mask;
};
