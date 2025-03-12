#include "world.h"

#include <glm/ext/vector_float2.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>

#include "block.h"
#include "chunk.h"

size_t Array3Hash::operator()(std::array<int32_t, 3> const& array3) const
{
    return static_cast<size_t>(array3[0]) << 44 |
           static_cast<size_t>(array3[1]) << 22 | array3[2];
}

World::World() : chunks{} {}

std::array<int32_t, 3> World::get_chunk_key(std::array<int64_t, 3> at)
{
    static auto calculate_chunk_key = [](int64_t at_n) -> int32_t {
        return at_n < 0 ? (at_n + 1) / g_chunk_size - 1 : at_n / g_chunk_size;
    };
    return {
        calculate_chunk_key(at[0]),
        calculate_chunk_key(at[1]),
        calculate_chunk_key(at[2])
    };
}

std::optional<std::array<std::array<int64_t, 3>, 2>>
World::get_chunk_and_block_key(std::array<int64_t, 3> at)
{
    std::array<int32_t, 3> const chunk_key{get_chunk_key(at)};

    // if chunk does not exist say block exists
    if (chunks.find(chunk_key) == chunks.end()) {
        return std::nullopt;
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

    std::array<int64_t, 3> const block_key{
        calculate_block_key(at[0]),
        calculate_block_key(at[1]),
        calculate_block_key(at[2])
    };

    std::array<int64_t, 3> ll_chunk_key{
        static_cast<int64_t>(chunk_key[0]),
        static_cast<int64_t>(chunk_key[1]),
        static_cast<int64_t>(chunk_key[2])
    };
    return {{ll_chunk_key, block_key}};
}

uint32_t World::test_block(std::array<int64_t, 3> at)
{
    auto const cb_opt = get_chunk_and_block_key(at);
    if (!cb_opt.has_value()) {
        return 0;
    }
    auto const [chunk_key, block_key] = get_chunk_and_block_key(at).value();
    std::array<int32_t, 3> l_chunk_key{
        static_cast<int32_t>(chunk_key[0]),
        static_cast<int32_t>(chunk_key[1]),
        static_cast<int32_t>(chunk_key[2])
    };
    return chunks[l_chunk_key].test_block_mask(block_key);
}

void World::remove_block(std::array<int64_t, 3> at)
{
    std::cout << "remove_block at: " << at[0] << " " << at[1] << " " << at[2]
              << "\n";
    auto const cb_opt = get_chunk_and_block_key(at);
    if (!cb_opt.has_value()) {
        return;
    }
    auto const [chunk_key, block_key] = get_chunk_and_block_key(at).value();
    std::array<int32_t, 3> l_chunk_key{
        static_cast<int32_t>(chunk_key[0]),
        static_cast<int32_t>(chunk_key[1]),
        static_cast<int32_t>(chunk_key[2])
    };
    chunks[l_chunk_key].set_block(block_key[0], block_key[1], block_key[2], 0);
    generate_mesh(l_chunk_key);
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
                    chunk.set_block(
                        i,
                        j,
                        k,
                        static_cast<uint32_t>(std::abs(noise) * g_blocks.size())
                    );
                }
            }
        }
    }

    chunks.emplace(std::make_pair(at, std::move(chunk)));
    generate_mesh(at);
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
    generate_mesh(at);
}

void World::generate_mesh(std::array<int32_t, 3> at)
{
    std::cout << "mesh_gen: " << at[0] << " " << at[1] << " " << at[2] << "\n";
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
    meshes[at] = std::move(faces);
}

std::vector<uint32_t> const* World::get_chunk_mesh(std::array<int32_t, 3> at)
{
    if (meshes.count(at)) {
        return &meshes[at];
    }
    return nullptr;
}
