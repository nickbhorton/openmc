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

static uint32_t rand_block_index()
{
    int min = 1;
    int max = g_blocks.size();
    uint32_t random = min + rand() % (max - min + 1);
    return random;
}

void World::generate_chunk(std::array<int32_t, 3> at)
{
    Chunk chunk{};
    for (uint32_t i = 0; i < g_chunk_size; i++) {
        for (uint32_t j = 0; j < g_chunk_size; j++) {
            for (uint32_t k = 0; k < g_chunk_size; k++) {
                float noise = glm::perlin(glm::vec3(
                    static_cast<float>(at[0]) +
                        static_cast<float>(i) / g_chunk_size,
                    static_cast<float>(at[1]) +
                        static_cast<float>(j) / g_chunk_size,
                    static_cast<float>(at[2]) +
                        static_cast<float>(k) / g_chunk_size
                ));
                if (std::abs(noise) > 0.3) {
                    chunk.set_block(i, j, k, rand_block_index());
                }
            }
        }
    }

    chunks.emplace(std::make_pair(at, std::move(chunk)));
}

void World::generate_debug_chunk(std::array<int32_t, 3> at)
{
    Chunk chunk{};
    for (uint32_t i = 0; i < g_blocks.size(); i++) {
        chunk.set_block(
            (1 + (i * 2) / (g_chunk_size - 2)) * 2,
            0,
            1 + (i * 2) % (g_chunk_size - 2),
            i + 1
        );
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
