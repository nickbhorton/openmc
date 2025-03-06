#include "vao.h"

VertexArrayObject::VertexArrayObject() : name{0}, shader_name{0}, moved{false}
{
    glGenVertexArrays(1, &name);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
    : name(other.name), shader_name(other.shader_name), moved(false)
{
    other.moved = true;
    for (size_t i = 0; i < other.owned_buffers.size(); i++) {
        this->owned_buffers.push_back(std::move(other.owned_buffers[i]));
    }
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other
) noexcept
{
    name = other.name;
    shader_name = other.shader_name;
    moved = false;
    other.moved = true;
    for (size_t i = 0; i < other.owned_buffers.size(); i++) {
        this->owned_buffers.push_back(std::move(other.owned_buffers[i]));
    }
    return *this;
}

VertexArrayObject::~VertexArrayObject()
{
    if (!moved) {
        glDeleteVertexArrays(1, &name);
    }
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

static GLint
get_attribute_location(GLint shader_name, std::string const& attribute_name)
{
    GLint const attrib_location =
        glGetAttribLocation(shader_name, attribute_name.c_str());
    if (attrib_location < 0) {
        std::cout << "vertex attrib '" << attribute_name << "' was not found"
                  << "\n";
    }
    return attrib_location;
}

static void attach_buffer_object_impl(
    std::string const& attribute_name,
    GLint size,
    GLenum type,
    GLenum buffer_bind_target,
    GLuint buffer_name,
    GLuint shader_name,
    GLuint vao_name,
    GLuint divisor,
    bool integer_type = false
)
{
    // bind vao and buffer
    glBindVertexArray(vao_name);
    glBindBuffer(buffer_bind_target, buffer_name);

    GLint const attrib_location =
        get_attribute_location(shader_name, attribute_name);
    if (integer_type) {
        glVertexAttribIPointer(
            attrib_location, // index
            size,            // number of numbers
            type,            // type
            0,               // stride
            BUFFER_OFFSET(0) // how far into the buffer is the fisrt num
        );
    } else {
        glVertexAttribPointer(
            attrib_location, // index
            size,            // number of numbers
            type,            // type
            GL_FALSE,        // normalized
            0,               // stride
            BUFFER_OFFSET(0) // how far into the buffer is the fisrt num
        );
    }
    glEnableVertexAttribArray(attrib_location);

    // if we are instancing set the divisor
    if (divisor > 0) {
        glVertexAttribDivisor(attrib_location, divisor);
    }
}

void VertexArrayObject::attach_buffer_object(
    std::string const& attribute_name,
    StaticBuffer& buffer,
    GLuint divisor
)
{
    attach_buffer_object_impl(
        attribute_name,
        buffer.get_attrib_size(),
        buffer.get_type(),
        buffer.get_bind_target(),
        buffer.get_name(),
        shader_name,
        name,
        divisor,
        buffer.get_type() == GL_UNSIGNED_INT || buffer.get_type() == GL_INT
    );
}
void VertexArrayObject::attach_buffer_object(
    std::string const& attribute_name,
    StaticBuffer&& buffer,
    GLuint divisor
)
{
    attach_buffer_object_impl(
        attribute_name,
        buffer.get_attrib_size(),
        buffer.get_type(),
        buffer.get_bind_target(),
        buffer.get_name(),
        shader_name,
        name,
        divisor,
        buffer.get_type() == GL_UNSIGNED_INT || buffer.get_type() == GL_INT
    );
    owned_buffers.push_back(std::move(buffer));
}
