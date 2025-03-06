#pragma once

#include <glad/gl.h>

#include <iostream>
#include <string>
#include <vector>

class ShaderObject
{
public:
    ShaderObject() = delete;
    ShaderObject(std::string const& shader_filename, GLenum shader_type);

    ShaderObject(ShaderObject const& other) = delete;
    ShaderObject& operator=(ShaderObject const& other) = delete;
    ShaderObject(ShaderObject&& other) noexcept;
    ShaderObject& operator=(ShaderObject&& other) noexcept;

    ~ShaderObject();

    GLuint get_name() const;

private:
    GLuint name;
    bool moved;
};

class ShaderProgram
{
public:
    ShaderProgram(std::vector<std::pair<std::string, GLenum>> shaders);

    ShaderProgram(ShaderProgram const& other) = delete;
    ShaderProgram operator=(ShaderProgram const& other) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ~ShaderProgram();

    template <typename T>
    void update_uniform(std::string const& uniform_name, T new_value);

    auto get_name() const -> GLuint;

    void bind();

private:
    GLuint name;
    std::vector<ShaderObject> sos;
    bool moved;

    void attatch(ShaderObject const& shader);
    void link();
};

template <typename T>
void ShaderProgram::update_uniform(std::string const& uniform_name, T new_value)
{
    std::cerr << "ShaderProgram uniform type not implemented\n";
}
