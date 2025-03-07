#pragma once

#include <glad/gl.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>
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
    bool moved;

    std::vector<ShaderObject> sos;

    void attatch(ShaderObject const& shader);
    void link();
};

template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    float new_val
);
template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    int new_val
);
template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::mat4 new_val
);
template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::vec2 new_val
);
template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::vec3 new_val
);
template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::vec4 new_val
);
template <typename T>
void ShaderProgram::update_uniform(std::string const& uniform_name, T new_value)
{
    std::cerr << "ShaderProgram uniform type for " << uniform_name
              << " not implemented\n";
}
