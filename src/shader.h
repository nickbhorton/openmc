#pragma once

#include <glad/gl.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include <vector>

class ShaderObject
{
    GLuint name;
    bool moved;

public:
    ShaderObject(std::string const& shader_filename, GLenum shader_type);
    ShaderObject() = delete;
    ~ShaderObject();

    ShaderObject(ShaderObject const& other) = delete;
    ShaderObject& operator=(ShaderObject const& other) = delete;

    ShaderObject(ShaderObject&& other) noexcept;
    ShaderObject& operator=(ShaderObject&& other) = delete;

    auto get_name() const -> GLuint;
};

class ShaderProgram
{
    GLuint name;
    std::vector<ShaderObject> sos;
    bool moved;

    void attatch(ShaderObject const& shader);

public:
    ShaderProgram(std::vector<std::pair<std::string, GLenum>> shaders);
    ~ShaderProgram();

    ShaderProgram(ShaderProgram const& other) = delete;
    ShaderProgram operator=(ShaderProgram const& other) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) = delete;

    auto get_name() const -> GLuint;

    void link();

    void bind();

    void update_uniform_1f(std::string const& uniform_name, GLfloat new_val);
    void update_uniform_1i(std::string const& uniform_name, GLint new_val);
    void update_uniform_mat4f(
        std::string const& uniform_name,
        glm::mat4 const& new_val
    );
    void update_uniform_vec2f(
        std::string const& uniform_name,
        glm::vec2 const& new_val
    );
    void update_uniform_vec3f(
        std::string const& uniform_name,
        glm::vec3 const& new_val
    );
    void update_uniform_vec4f(
        std::string const& uniform_name,
        glm::vec4 const& new_val
    );
};
