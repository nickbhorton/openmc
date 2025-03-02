#pragma once

#include <array>
#include <tuple>
#include <vector>

std::tuple<
    std::vector<std::array<float, 3>>,
    std::vector<std::array<float, 2>>,
    std::vector<std::array<float, 3>>>
cube_geometry();
