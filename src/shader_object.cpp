#include "shader.h"

#include <fstream>
#include <iostream>

static void report_shader_info_log(GLuint name, std::string const& msg)
{
    GLsizei info_log_length{};
    glGetShaderiv(name, GL_INFO_LOG_LENGTH, &info_log_length);
    std::string error_log("\0", info_log_length);
    glGetShaderInfoLog(
        name,
        info_log_length,
        &info_log_length,
        (GLchar*)error_log.data()
    );
    std::cerr << "shader_info_log" << msg << std::string(": ") << error_log
              << std::endl;
}

ShaderObject::ShaderObject(
    std::string const& shader_filename,
    GLenum shader_type
)
    : name{0}, moved(false)
{
    name = glCreateShader(shader_type);
    if (!name) {
        std::cerr << "shader could not be created\n";
        std::exit(1);
    }

    std::fstream file{};
    file.open(shader_filename);
    if (file.bad() || file.fail() || !file.good()) {
        std::cerr << shader_filename << " was bad" << std::endl;
        std::exit(1);
    }

    std::string shader_source{};
    char file_byte{};
    while (!file.eof()) {
        file_byte = file.get();
        if (file.eof()) {
            break;
        }
        shader_source.push_back(file_byte);
    }
    const char* shader_source_ptr{shader_source.data()};
    const GLint shader_source_length{static_cast<GLint>(shader_source.size())};

    glShaderSource(name, 1, &shader_source_ptr, &shader_source_length);
    glCompileShader(name);

    GLint compile_status{};
    glGetShaderiv(name, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        report_shader_info_log(
            name,
            "Shader Compilation Error " + shader_filename
        );
        std::exit(1);
    }
}

ShaderObject::ShaderObject(ShaderObject&& other) noexcept
    : name(other.name), moved(false)
{
    other.moved = true;
}

ShaderObject& ShaderObject::operator=(ShaderObject&& other) noexcept
{
    this->name = other.name;
    this->moved = false;
    other.moved = true;
    return *this;
}

ShaderObject::~ShaderObject()
{
    if (!moved) {
        glDeleteShader(name);
    }
}

GLuint ShaderObject::get_name() const { return name; }
