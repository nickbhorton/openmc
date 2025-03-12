#pragma once

#include <array>
#include <vector>

std::vector<std::array<float, 3>>
intersections(std::array<float, 3> pos, std::array<float, 3> dir, int radius);
