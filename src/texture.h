#pragma once

#include <glad/gl.h>

#include <glm/ext/vector_int2.hpp>

#include <string>

#include "image.h"

class Texture
{
    GLuint name;
    int number_of_channels;

public:
    glm::ivec2 size;

    Texture(std::string const& path, int active_texture);
    Texture(Image const& img, int active_texture);
    ~Texture();
};
