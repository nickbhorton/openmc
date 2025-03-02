#include "geometry.h"

std::tuple<
    std::vector<std::array<float, 3>>,
    std::vector<std::array<float, 2>>,
    std::vector<std::array<float, 3>>>
cube_geometry()
{
    size_t constexpr vertex_count{36};
    std::array<std::array<float, 3>, 8> constexpr positions_gen{
        {{-1, -1, -1},
         {1, -1, -1},
         {1, 1, -1},
         {-1, 1, -1},
         {-1, -1, 1},
         {1, -1, 1},
         {1, 1, 1},
         {-1, 1, 1}}
    };
    std::array<std::array<float, 2>, 4> constexpr uvs_gen{
        {{0, 0}, {1, 0}, {1, 1}, {0, 1}}
    };

    std::array<std::array<float, 3>, 6> constexpr normals_gen{
        {{0, 0, 1}, {1, 0, 0}, {0, 0, -1}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}}
    };

    std::array<size_t, vertex_count> constexpr position_indexes = {
        0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1
    };
    std::array<size_t, vertex_count / 6> constexpr uv_indexes =
        {0, 1, 3, 3, 1, 2};

    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 2>> uvs;
    std::vector<std::array<float, 3>> normals;

    for (int i = 0; i < vertex_count; i++) {
        positions.push_back(positions_gen[position_indexes[i]]);
        uvs.push_back(uvs_gen[uv_indexes[i % 6]]);
        normals.push_back(normals_gen[position_indexes[i / 6]]);
    }
    return {positions, uvs, normals};
}
