#pragma once

#include <array>
#include <string>
#include <vector>

class Image
{
    unsigned char* data;
    int number_of_channels;

public:
    std::array<int, 2> size;

    Image(std::string const& path);
    Image(std::vector<Image const*> const& to_stitch, size_t cols_per_row);
    ~Image();

    Image(Image const& other) = delete;
    Image(Image&& other) noexcept = delete;
    Image& operator=(Image const& other) = delete;
    Image& operator=(Image&& other) noexcept = delete;

    std::array<unsigned char, 3> get_pixel(unsigned int x, unsigned int y);
};
