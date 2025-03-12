#include "chunk.h"

Chunk::Chunk() : block_mask{} {}

static uint32_t calculate_index(uint32_t x, uint32_t y, uint32_t z)
{
    return x + (y * g_chunk_size * g_chunk_size) + (z * g_chunk_size);
}

uint32_t Chunk::test_block_mask(int32_t x, int32_t y, int32_t z)
{
    // if testing outside of chunk give false
    // this is a stop gap before introducing multiple chunks
    if (x > g_chunk_size - 1 || y > g_chunk_size - 1 || z > g_chunk_size - 1) {
        return false;
    }
    if (x < 0 || y < 0 || z < 0) {
        return false;
    }

    uint32_t const index = calculate_index(x, y, z);
    return block_mask[index];
}

uint32_t Chunk::test_block_mask(std::array<int32_t, 3> at)
{
    return test_block_mask(at[0], at[1], at[2]);
}

uint32_t Chunk::test_block_mask(std::array<int64_t, 3> at)
{
    return test_block_mask(at[0], at[1], at[2]);
}

void Chunk::set_block(uint32_t x, uint32_t y, uint32_t z, uint32_t block)
{
    uint32_t const index = calculate_index(x, y, z);
    block_mask[index] = static_cast<unsigned char>(block);
}

void Chunk::unset_block(uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t const index = calculate_index(x, y, z);
    block_mask[index] = 0;
}
