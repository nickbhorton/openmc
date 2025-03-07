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

void World::test_block(std::array<int64_t, 3> at)
{
    static auto calculate_chunk_key = [](int64_t at_n) -> int32_t {
        return at_n < 0 ? (at_n + 1) / g_chunk_size - 1 : at_n / g_chunk_size;
    };
    std::array<int32_t, 3> chunk_key{
        calculate_chunk_key(at[0]),
        calculate_chunk_key(at[1]),
        calculate_chunk_key(at[2])
    };
    static auto calculate_block_key = [](int64_t at_n) -> int32_t {
        return at_n < 0 ? (32 - (-at_n % 33)) : at_n % g_chunk_size;
    };
    std::array<int32_t, 3> block_key{
        calculate_block_key(at[0]),
        calculate_block_key(at[1]),
        calculate_block_key(at[2])
    };

    std::cout << "at: " << at[0] << " " << at[1] << " " << at[2] << "\t";
    std::cout << "chunk_at: " << chunk_key[0] << " " << chunk_key[1] << " "
              << chunk_key[2] << "\t";
    std::cout << "block_at: " << block_key[0] << " " << block_key[1] << " "
              << block_key[2] << "\n";
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
                    block::sand
                );
            }
            chunk.set_block(i, uint_noise, j, block::sand);
        }
    }

    chunks.emplace(std::make_pair(at, std::move(chunk)));
}
