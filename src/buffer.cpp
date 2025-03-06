#include "buffer.h"

#include <array>

StaticBuffer::StaticBuffer(StaticBuffer&& other) noexcept
    : moved(false), bind_target(other.bind_target), data_size(other.data_size),
      name(other.name)
{
    other.moved = true;
}

StaticBuffer& StaticBuffer::operator=(StaticBuffer&& other) noexcept
{
    this->moved = false;
    this->bind_target = other.bind_target;
    this->name = other.name;
    this->data_size = other.data_size;
    other.moved = true;
    return *this;
}

void StaticBuffer::bind() { glBindBuffer(bind_target, name); }

GLenum StaticBuffer::get_bind_target() { return bind_target; }

GLsizei StaticBuffer::byte_count() { return data_size; }

GLenum StaticBuffer::get_type() { return data_type; }

GLint StaticBuffer::get_attrib_size() { return attrib_size; }

StaticBuffer::~StaticBuffer()
{
    if (!moved) {
        glDeleteBuffers(1, &name);
    }
}

GLuint StaticBuffer::get_name() { return name; }

// specialized data
template <>
StaticBuffer::StaticBuffer(
    std::vector<std::array<float, 3>> buffer_data,
    GLenum bind_target
)
    : moved(false), bind_target(bind_target),
      data_size(sizeof(std::array<float, 3>) * buffer_data.size()),
      data_type{GL_FLOAT}, attrib_size{3}
{
    glCreateBuffers(1, &name);
    glNamedBufferStorage(name, this->data_size, buffer_data.data(), 0);
}

template <>
StaticBuffer::StaticBuffer(
    std::vector<uint32_t> buffer_data,
    GLenum bind_target
)
    : moved(false), bind_target(bind_target),
      data_size(sizeof(uint32_t) * buffer_data.size()),
      data_type{GL_UNSIGNED_INT}, attrib_size{1}
{
    glCreateBuffers(1, &name);
    glNamedBufferStorage(name, this->data_size, buffer_data.data(), 0);
}
