#include "image.h"

#include <iostream>

#include <stb/stb_image.h>
#include <string.h>

Image::Image(std::string const& path)
    : size{}, number_of_channels{}, is_stb{true}, data{nullptr}
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &size[0], &size[1], &number_of_channels, 0);
    if (!data) {
        std::cerr << path << " failed to load\n";
    } else {
        /*
        std::cout << path << " " << size[0] << "x" << size[1] << " "
                  << number_of_channels << "\n";
        */
    }
}

Image::~Image()
{
    if (is_stb) {
        stbi_image_free(data);
    } else {
        free(data);
    }
}

std::array<unsigned char, 3>
Image::get_pixel(unsigned int x, unsigned int y) const
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
        /*
    std::cerr << "pixel from image with strange number of channels "
              << number_of_channels << "\n";
        */
        return {data[index], data[index + 1], data[index + 2]};
    }
}

Image::Image(std::vector<Image const*> const& to_stitch, size_t square_discr)
    : size{}, number_of_channels{}, is_stb{false}, data{nullptr}
{
    if (to_stitch.size() == 0) {
        return;
    }
    std::array<int, 2> sub_img_size = to_stitch[0]->size;
    // making sure all sub images are the same size;
    for (auto const& img : to_stitch) {
        if (img->size != sub_img_size) {
            std::cerr << "different sized images found in a stiched image\n";
            return;
        }
    }

    size[0] = square_discr * sub_img_size[0];
    size[1] = square_discr * sub_img_size[1];
    number_of_channels = 3;

    data = static_cast<unsigned char*>(malloc(3 * size[0] * size[1]));
    memset(data, 0, 3 * size[0] * size[1]);

    for (size_t i = 0; i < square_discr; i++) {
        for (size_t j = 0; j < square_discr; j++) {
            for (size_t y = 0; y < static_cast<size_t>(sub_img_size[1]); y++) {
                for (size_t x = 0; x < static_cast<size_t>(sub_img_size[0]);
                     x++) {
                    size_t image_idx = i * square_discr + j;
                    if (image_idx < to_stitch.size()) {
                        auto pix = to_stitch[image_idx]->get_pixel(x, y);
                        size_t row = (i * sub_img_size[0]) + y;
                        size_t col = (j * sub_img_size[1]) + x;
                        size_t idx = ((row * size[1] + col)) * 3;
                        data[idx + 0] = pix[0];
                        data[idx + 1] = pix[1];
                        data[idx + 2] = pix[2];
                    }
                }
            }
        }
    }
}
