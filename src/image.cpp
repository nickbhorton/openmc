#include "image.h"

#include <iostream>

#include <stb/stb_image.h>

Image::Image(std::string const& path)
    : data{nullptr}, size{}, number_of_channels{}
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &size[0], &size[1], &number_of_channels, 0);
    if (!data) {
        std::cerr << path << " failed to load\n";
    } else {
        std::cout << path << " " << size[0] << "x" << size[1] << " "
                  << number_of_channels << "\n";
    }
}

Image::~Image() { stbi_image_free(data); }

std::array<unsigned char, 3> Image::get_pixel(unsigned int x, unsigned int y)
{
    size_t index = (y * size[1] + x) * number_of_channels;
    switch (number_of_channels) {
    case 1:
        return {data[index], data[index], data[index]};
        break;
    case 2:
        return {data[index], data[index + 1], 0};
        break;
    case 3:
        return {data[index], data[index + 1], data[index + 2]};
        break;
    default:
        std::cerr << "pixel from image with strange number of channels\n";
        return {0, 0, 0};
    }
}

Image::Image(std::vector<Image const*> const& to_stitch, size_t cols_per_row)
    : data{nullptr}, size{}, number_of_channels{}
{
    if (to_stitch.size() == 0) {
        return;
    }
    std::array<int, 2> first_img_size = to_stitch[0]->size;
    for (auto const& img : to_stitch) {
        if (img->size != first_img_size) {
            std::cerr << "different sized images found in a stiched image\n";
            return;
        }
    }
    size[0] = (to_stitch.size() / cols_per_row) * first_img_size[0];
    size[1] = cols_per_row * first_img_size[1];
    std::cout << size[0] << "x" << size[1] << "\n";
}
