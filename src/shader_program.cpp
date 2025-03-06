#include "shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void report_program_info_log(GLuint name, std::string const& msg)
{
    GLsizei info_log_length{};
    glGetProgramiv(name, GL_INFO_LOG_LENGTH, &info_log_length);
    std::string error_log("\0", info_log_length);
    glGetProgramInfoLog(
        name,
        info_log_length,
        &info_log_length,
        error_log.data()
    );
    std::cerr << "shader_program_info_log" << msg << std::string(": ")
              << error_log << std::endl;
}

ShaderProgram::ShaderProgram(std::vector<std::pair<std::string, GLenum>> shaders
)
    : moved{false}
{
    name = glCreateProgram();
    if (!name) {
        std::cerr << "glCreateProgram could not create shader" << std::endl;
        std::exit(1);
    }
    for (auto const& [filename, shader_type] : shaders) {
        ShaderObject shader_object(filename, shader_type);
        sos.push_back(std::move(shader_object));
    }
    for (auto const& so : sos) {
        this->attatch(so);
    }
    this->link();
}

ShaderProgram::~ShaderProgram()
{
    if (!moved) {
        glDeleteProgram(name);
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : moved{false}, name{other.name}
{
    other.moved = true;
    for (size_t i = 0; i < other.sos.size(); i++) {
        sos.push_back(std::move(other.sos[i]));
    }
    other.sos.clear();
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    this->moved = false;
    this->name = other.name;
    other.moved = true;
    for (size_t i = 0; i < other.sos.size(); i++) {
        this->sos.push_back(std::move(other.sos[i]));
    }
    other.sos.clear();
    return *this;
}

void ShaderProgram::attatch(ShaderObject const& shader)
{
    glAttachShader(name, shader.get_name());
}

void ShaderProgram::link()
{
    glLinkProgram(name);
    GLint link_status{};
    glGetProgramiv(name, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        report_program_info_log(name, "Shader Link Error");
        throw 1;
    }
}

void ShaderProgram::bind() { glUseProgram(name); }

auto ShaderProgram::get_name() const -> GLuint { return name; }

template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    float new_val
)
{
    this->bind();
    GLint location = glGetUniformLocation(name, uniform_name.c_str());
    glUniform1f(location, new_val);
}

template <>
void ShaderProgram::update_uniform(std::string const& uniform_name, int new_val)
{
    this->bind();
    GLint location = glGetUniformLocation(name, uniform_name.c_str());
    glUniform1i(location, new_val);
}

template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::mat4 const& new_val
)
{
    this->bind();
    GLint location = glGetUniformLocation(name, uniform_name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(new_val));
}

template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::vec2 const& new_val
)
{
    this->bind();
    GLint location = glGetUniformLocation(name, uniform_name.c_str());
    glUniform2fv(location, 1, glm::value_ptr(new_val));
}

template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::vec3 const& new_val
)
{
    this->bind();
    GLint location = glGetUniformLocation(name, uniform_name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(new_val));
}

template <>
void ShaderProgram::update_uniform(
    std::string const& uniform_name,
    glm::vec4 const& new_val
)
{
    this->bind();
    GLint location = glGetUniformLocation(name, uniform_name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(new_val));
}
