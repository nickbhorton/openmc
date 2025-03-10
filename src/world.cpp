#include "world.h"

#include <glm/ext/vector_float2.hpp>
#include <glm/gtc/noise.hpp>

#include "block.h"
#include "chunk.h"

size_t Array3Hash::operator()(std::array<int32_t, 3> const& array3) const
{
    return static_cast<size_t>(array3[0]) << 44 |
           static_cast<size_t>(array3[1]) << 22 | array3[2];
}

World::World() : chunks{} {}

uint32_t World::test_block(std::array<int64_t, 3> at)
{
    static auto calculate_chunk_key = [](int64_t at_n) -> int32_t {
        return at_n < 0 ? (at_n + 1) / g_chunk_size - 1 : at_n / g_chunk_size;
    };
    std::array<int32_t, 3> const chunk_key{
        calculate_chunk_key(at[0]),
        calculate_chunk_key(at[1]),
        calculate_chunk_key(at[2])
    };
    // std::cout << chunk_key[0] << " " << chunk_key[2] << "\n";

    // if chunk does not exist say block exists
    if (chunks.find(chunk_key) == chunks.end()) {
        return 0;
    }

    static auto calculate_block_key = [](int64_t at_n) -> int32_t {
        if (at_n < 0) {
            if (-at_n % g_chunk_size == 0) {
                return -at_n % g_chunk_size;
            }
            return 32 - (-at_n % g_chunk_size);
        }
        return at_n % g_chunk_size;
    };
    std::array<int32_t, 3> const block_key{
        calculate_block_key(at[0]),
        calculate_block_key(at[1]),
        calculate_block_key(at[2])
    };
    return chunks[chunk_key].test_block_mask(block_key);
}

void World::generate_chunk(std::array<int32_t, 3> at)
{
    Chunk chunk{};
    for (uint32_t i = 0; i < g_chunk_size; i++) {
        for (uint32_t j = 0; j < g_chunk_size; j++) {
            float noise = glm::perlin(glm::vec2(
                static_cast<float>(at[0]) +
                    static_cast<float>(i) / g_chunk_size,
                static_cast<float>(at[2]) + static_cast<float>(j) / g_chunk_size
            ));
            uint32_t uint_noise =
                3 + static_cast<uint32_t>(std::abs(27.0 * noise));

            uint32_t constexpr dirt_width = 3;
            for (uint32_t k = 0; k < uint_noise - dirt_width; k++) {
                chunk.set_block(i, k, j, block::stone);
            }
            for (uint32_t k = 0; k < dirt_width; k++) {
                chunk.set_block(
                    i,
                    (uint_noise - dirt_width) + k,
                    j,
                    block::stone
                );
            }
            chunk.set_block(i, uint_noise, j, block::stone);
        }
    }

    chunks.emplace(std::make_pair(at, std::move(chunk)));
}

std::vector<uint32_t> World::get_chunk_mesh(std::array<int32_t, 3> at)
{
    std::vector<uint32_t> faces;
    for (int64_t xi = 0; xi < g_chunk_size; xi++) {
        for (int64_t zi = 0; zi < g_chunk_size; zi++) {
            for (int64_t yi = 0; yi < g_chunk_size; yi++) {
                int64_t x = at[0] * g_chunk_size + xi;
                int64_t y = at[1] * g_chunk_size + yi;
                int64_t z = at[2] * g_chunk_size + zi;

                uint32_t block_index = test_block({x, y, z});
                if (block_index) {
                    if (!test_block({x, y - 1, z})) {
                        faces.push_back(g_blocks[block_index - 1].get_face(
                            {static_cast<unsigned int>(xi),
                             static_cast<unsigned int>(yi),
                             static_cast<unsigned int>(zi)},
                            Direction::Down
                        ));
                    }
                    if (!test_block({x, y + 1, z})) {
                        faces.push_back(g_blocks[block_index - 1].get_face(
                            {static_cast<unsigned int>(xi),
                             static_cast<unsigned int>(yi),
                             static_cast<unsigned int>(zi)},
                            Direction::Up
                        ));
                    }
                    if (!test_block({x, y, z + 1})) {
                        faces.push_back(g_blocks[block_index - 1].get_face(
                            {static_cast<unsigned int>(xi),
                             static_cast<unsigned int>(yi),
                             static_cast<unsigned int>(zi)},
                            Direction::North
                        ));
                    }
                    if (!test_block({x - 1, y, z})) {
                        faces.push_back(g_blocks[block_index - 1].get_face(
                            {static_cast<unsigned int>(xi),
                             static_cast<unsigned int>(yi),
                             static_cast<unsigned int>(zi)},
                            Direction::East
                        ));
                    }
                    if (!test_block({x, y, z - 1})) {
                        faces.push_back(g_blocks[block_index - 1].get_face(
                            {static_cast<unsigned int>(xi),
                             static_cast<unsigned int>(yi),
                             static_cast<unsigned int>(zi)},
                            Direction::South
                        ));
                    }
                    if (!test_block({x + 1, y, z})) {
                        faces.push_back(g_blocks[block_index - 1].get_face(
                            {static_cast<unsigned int>(xi),
                             static_cast<unsigned int>(yi),
                             static_cast<unsigned int>(zi)},
                            Direction::West
                        ));
                    }
                }
            }
        }
    }
    return faces;
}
