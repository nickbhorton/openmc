#include "chunk.h"

bool Chunk::test_block_mask(uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t const index = x + (y * g_chunk_size[1]) + (z * g_chunk_size[2]);
    return block_mask[index / 64] & (static_cast<uint64_t>(1) << (index % 64));
}

void Chunk::set_block_mask(uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t const index = x + (y * g_chunk_size[1]) + (z * g_chunk_size[2]);
    block_mask[index / 64] |= (static_cast<uint64_t>(1) << (index % 64));
}

Chunk::Chunk() : block_mask{} {}
