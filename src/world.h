#pragma once

#include <array>
#include <unordered_map>

#include "chunk.h"

struct Array3Hash {
    size_t operator()(std::array<int32_t, 3> const& array3) const;
};

class World
{
public:
    World();

    uint32_t test_block(std::array<int64_t, 3> at);
    std::vector<uint32_t> get_chunk_mesh(std::array<int32_t, 3> at);
    void generate_chunk(std::array<int32_t, 3> at);
    void generate_debug_chunk(std::array<int32_t, 3> at);

private:
    std::unordered_map<std::array<int32_t, 3>, Chunk, Array3Hash> chunks;
};
