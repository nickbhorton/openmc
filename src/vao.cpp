#include "vao.h"

VertexArrayObject::VertexArrayObject() : shader_name{0}
{
    glGenVertexArrays(1, &name);
}
void VertexArrayObject::attach_shader(ShaderProgram const& shader)
{
    shader_name = shader.get_name();
}

void VertexArrayObject::bind()
{
    if (shader_name) {
        glUseProgram(shader_name);
    }
    glBindVertexArray(name);
}

VertexArrayObject::~VertexArrayObject()
{
    if (!moved) {
        glDeleteVertexArrays(1, &name);
    }
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
    : moved(false), name(other.name), shader_name(other.shader_name)
{
    other.moved = true;
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other
) noexcept
{
    moved = false;
    name = other.name;
    shader_name = other.shader_name;
    other.moved = true;
    return *this;
}

template <>
void VertexArrayObject::attach_buffer_object(
    std::string const& attribute_name,
    StaticBuffer<std::array<float, 3>>& buffer,
    GLuint divisor
)
{
    GLint const attrib_location =
        glGetAttribLocation(shader_name, attribute_name.c_str());
    if (attrib_location < 0) {
        std::cout << "vertex attrib '" << attribute_name << "' was not found"
                  << "\n";
    }
    glBindVertexArray(name);
    buffer.bind();
    glVertexAttribPointer(
        attrib_location, // index
        3,               // number of numbers
        GL_FLOAT,        // type
        GL_FALSE,        // normalized
        0,
        BUFFER_OFFSET(0) // how far into the buffer is the fisrt num
    );
    glEnableVertexAttribArray(attrib_location);
    if (divisor > 0) {
        glVertexAttribDivisor(attrib_location, divisor);
    }
}

template <>
void VertexArrayObject::attach_buffer_object(
    std::string const& attribute_name,
    StaticBuffer<std::array<float, 2>>& buffer,
    GLuint divisor
)
{
    GLint const attrib_location =
        glGetAttribLocation(shader_name, attribute_name.c_str());
    if (attrib_location < 0) {
        std::cout << "vertex attrib '" << attribute_name << "' was not found"
                  << "\n";
    }
    glBindVertexArray(name);
    buffer.bind();
    glVertexAttribPointer(
        attrib_location, // index
        2,               // number of numbers
        GL_FLOAT,        // type
        GL_FALSE,        // normalized
        0,
        BUFFER_OFFSET(0) // how far into the buffer is the fisrt num
    );
    glEnableVertexAttribArray(attrib_location);
    if (divisor > 0) {
        glVertexAttribDivisor(attrib_location, divisor);
    }
}

template <>
void VertexArrayObject::attach_buffer_object(
    std::string const& attribute_name,
    StaticBuffer<uint32_t>& buffer,
    GLuint divisor
)
{
    GLint const attrib_location =
        glGetAttribLocation(shader_name, attribute_name.c_str());
    if (attrib_location < 0) {
        std::cout << "vertex attrib '" << attribute_name << "' was not found"
                  << "\n";
    }
    glBindVertexArray(name);
    buffer.bind();
    glVertexAttribIPointer(
        attrib_location, // index
        1,               // number of numbers
        GL_UNSIGNED_INT, // type
        0,
        BUFFER_OFFSET(0) // how far into the buffer is the fisrt num
    );
    glEnableVertexAttribArray(attrib_location);
    if (divisor > 0) {
        glVertexAttribDivisor(attrib_location, divisor);
    }
}
