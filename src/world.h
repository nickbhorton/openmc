#pragma once

#include <array>
#include <optional>
#include <unordered_map>

#include "chunk.h"

struct Array3Hash {
    size_t operator()(std::array<int32_t, 3> const& array3) const;
};

class World
{
public:
    World();

    std::vector<uint32_t> const* get_chunk_mesh(std::array<int32_t, 3> at);
    void generate_chunk(std::array<int32_t, 3> at);
    void generate_debug_chunk(std::array<int32_t, 3> at);

    uint32_t test_block(std::array<int64_t, 3> at);
    void remove_block(std::array<int64_t, 3> at);
    std::array<int32_t, 3> get_chunk_key(std::array<int64_t, 3> at);

private:
    void generate_mesh(std::array<int32_t, 3> at);
    std::optional<std::array<std::array<int64_t, 3>, 2>>
    get_chunk_and_block_key(std::array<int64_t, 3> at);

    std::unordered_map<std::array<int32_t, 3>, Chunk, Array3Hash> chunks;
    std::
        unordered_map<std::array<int32_t, 3>, std::vector<uint32_t>, Array3Hash>
            meshes;
};
