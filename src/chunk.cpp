#include "chunk.h"

#include "block.h"

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

static Block const stone(
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {TextureRotation::CW180,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::CW270,
     TextureRotation::CW180,
     TextureRotation::CW90}
);

static Block const sand(
    {{{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}}},
    {TextureRotation::CW180,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::CW270,
     TextureRotation::CW180,
     TextureRotation::CW90}
);

static Block const grass(
    {{{2, 0}, {2, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}}},
    {TextureRotation::CW180,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::CW270,
     TextureRotation::CW180,
     TextureRotation::CW90}
);

static Block const dirt(
    {{{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}},
    {TextureRotation::CW180,
     TextureRotation::None,
     TextureRotation::None,
     TextureRotation::CW270,
     TextureRotation::CW180,
     TextureRotation::CW90}
);

static Block const* blocks[4];
static bool blocks_init = false;

std::vector<uint32_t> Chunk::mesh()
{

    if (!blocks_init) {
        blocks[block::stone - 1] = &stone;
        blocks[block::sand - 1] = &sand;
        blocks[block::grass - 1] = &grass;
        blocks[block::dirt - 1] = &dirt;
        blocks_init = true;
    }

    std::vector<uint32_t> faces;
    for (uint32_t x = 0; x < g_chunk_size; x++) {
        for (uint32_t z = 0; z < g_chunk_size; z++) {
            for (uint32_t y = 0; y < g_chunk_size; y++) {
                uint32_t block_index = test_block_mask(x, y, z);
                if (block_index) {
                    if (!test_block_mask(x, y - 1, z)) {
                        faces.push_back(blocks[block_index - 1]->get_face(
                            {x, y, z},
                            Direction::Down
                        ));
                    }
                    if (!test_block_mask(x, y + 1, z)) {
                        faces.push_back(blocks[block_index - 1]->get_face(
                            {x, y, z},
                            Direction::Up
                        ));
                    }
                    if (!test_block_mask(x, y, z + 1)) {
                        faces.push_back(blocks[block_index - 1]->get_face(
                            {x, y, z},
                            Direction::North
                        ));
                    }
                    if (!test_block_mask(x - 1, y, z)) {
                        faces.push_back(blocks[block_index - 1]->get_face(
                            {x, y, z},
                            Direction::East
                        ));
                    }
                    if (!test_block_mask(x, y, z - 1)) {
                        faces.push_back(blocks[block_index - 1]->get_face(
                            {x, y, z},
                            Direction::South
                        ));
                    }
                    if (!test_block_mask(x + 1, y, z)) {
                        faces.push_back(blocks[block_index - 1]->get_face(
                            {x, y, z},
                            Direction::West
                        ));
                    }
                }
            }
        }
    }
    return faces;
}
