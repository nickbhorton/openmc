#pragma once

#include <glad/gl.h>

#include <glm/ext/vector_int2.hpp>

#include <string>

class Texture
{
    GLuint name;
    int number_of_channels;

public:
    glm::ivec2 size;

    Texture(std::string const& path, int active_texture);
    ~Texture();
};
