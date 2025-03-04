#include "chunk.h"

static uint32_t calculate_index(uint32_t x, uint32_t y, uint32_t z)
{
    return x + (y * g_chunk_size * g_chunk_size) + (z * g_chunk_size);
}

bool Chunk::test_block_mask(uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t const index = calculate_index(x, y, z);
    return block_mask[index / 64] & (static_cast<uint64_t>(1) << (index % 64));
}

void Chunk::set_block_mask(uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t const index = calculate_index(x, y, z);
    block_mask[index / 64] |= (static_cast<uint64_t>(1) << (index % 64));
}

Chunk::Chunk() : block_mask{} {}
