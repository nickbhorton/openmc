#include "texture.h"

#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(std::string const& path, int active_texture)
{

    glGenTextures(1, &name);
    glActiveTexture(active_texture);
    glBindTexture(GL_TEXTURE_2D, name);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data =
        stbi_load(path.c_str(), &size[0], &size[1], &number_of_channels, 0);

    if (!data) {
        std::cerr << "texture " << path << " could not be found" << std::endl;
        return;
    }

    GLenum channels;
    if (number_of_channels == 4) {
        channels = GL_RGBA;
    } else if (number_of_channels == 3) {
        channels = GL_RGB;
    } else if (number_of_channels == 2) {
        channels = GL_RG;
    } else if (number_of_channels == 1) {
        channels = GL_RED;
    } else {

        std::cerr << "texture has unsupported number of channels "
                  << number_of_channels << "\n";
    }
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        size[0],
        size[1],
        0,
        channels,
        GL_UNSIGNED_BYTE,
        data
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture() { glDeleteTextures(1, &name); }
