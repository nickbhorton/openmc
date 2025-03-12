#include "intersect.h"

#include <algorithm>
#include <cmath>
#include <iostream>

static float distance(std::array<float, 3> p1, std::array<float, 3> p2)
{
    float r{static_cast<float>(std::sqrt(
        std::pow(p2[0] - p1[0], 2.0) + std::pow(p2[1] - p1[1], 2.0) +
        std::pow(p2[2] - p1[2], 2.0)
    ))};
    return r;
}

static std::vector<std::array<float, 3>> interections_helper(
    std::array<float, 3> pos,
    std::array<float, 3> dir,
    std::array<size_t, 3> ord,
    int radius
)
{
    float theta_2_1 = std::atan2(dir[1], dir[0]);
    float theta_3_1 = std::atan2(dir[2], dir[0]);
    int i = 0;
    std::vector<std::array<float, 3>> result_vector{};
    while (true) {
        std::array<float, 3> result{};
        result[0] = (std::ceil(pos[0]) + i);
        result[1] = pos[1] + (result[0] - pos[0]) * std::tan(theta_2_1);
        result[2] = pos[2] + (result[0] - pos[0]) * std::tan(theta_3_1);
        std::array<float, 3> reordered_result{
            result[ord[0]],
            result[ord[1]],
            result[ord[2]]
        };
        if (distance(pos, reordered_result) > radius) {
            break;
        }
        result_vector.push_back(reordered_result);
        i++;
    }
    return result_vector;
}

struct CompairIntersections {
    std::array<float, 3> pos;
    bool operator()(std::array<float, 3> p1, std::array<float, 3> p2)
    {
        return distance(p1, pos) < distance(p2, pos);
    }
};

std::vector<std::array<float, 3>>
intersections(std::array<float, 3> pos, std::array<float, 3> dir, int radius)
{
    std::cout << "position: (";
    std::cout << pos[0] << ", ";
    std::cout << pos[1] << ", ";
    std::cout << pos[2] << ")\n";
    std::cout << "direction: (";
    std::cout << dir[0] << ", ";
    std::cout << dir[1] << ", ";
    std::cout << dir[2] << ")\n";

    std::vector<std::array<float, 3>> result_vector{};
    auto ord1{interections_helper(
        {pos[0], pos[1], pos[2]},
        {dir[0], dir[1], dir[2]},
        {0, 1, 2},
        radius
    )};
    for (auto const& v : ord1) {
        result_vector.push_back(v);
    }
    auto ord2{interections_helper(
        {pos[1], pos[2], pos[0]},
        {dir[1], dir[2], dir[0]},
        {2, 0, 1},
        radius
    )};
    for (auto const& v : ord2) {
        result_vector.push_back(v);
    }
    auto ord3{interections_helper(
        {pos[2], pos[0], pos[1]},
        {dir[2], dir[0], dir[1]},
        {1, 2, 0},
        radius
    )};
    for (auto const& v : ord3) {
        result_vector.push_back(v);
    }

    // sort based on distance from position
    CompairIntersections comp_op{.pos = pos};
    std::sort(result_vector.begin(), result_vector.end(), comp_op);

    for (auto const& v : result_vector) {
        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")\n";
        // std::cout << distance(v, pos) << "\n";
    };

    return result_vector;
}
