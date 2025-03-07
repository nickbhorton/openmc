#pragma once

#include <glad/gl.h>

#include <array>
#include <string>

#include "image.h"

class Texture
{
public:
    Texture(std::string const& path, int active_texture);
    Texture(Image const& img, int active_texture);
    ~Texture();

private:
    GLuint name;
    std::array<int, 2> size;
    int number_of_channels;
};
