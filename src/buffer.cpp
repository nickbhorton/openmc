#include "buffer.h"

#include <array>

StaticBuffer::StaticBuffer(StaticBuffer&& other) noexcept
    : name(other.name), bind_target(other.bind_target),
      data_type(other.data_type), attrib_size(other.attrib_size),
      data_size(other.data_size), moved(false)
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

StaticBuffer::~StaticBuffer()
{
    if (!moved) {
        glDeleteBuffers(1, &name);
    }
}

void StaticBuffer::bind() { glBindBuffer(bind_target, name); }

GLenum StaticBuffer::get_bind_target() const { return bind_target; }

size_t StaticBuffer::byte_count() const { return data_size; }

GLenum StaticBuffer::get_type() const { return data_type; }

GLint StaticBuffer::get_attrib_size() const { return attrib_size; }

GLuint StaticBuffer::get_name() const { return name; }

// specialized data
template <>
StaticBuffer::StaticBuffer(
    std::vector<std::array<float, 3>> buffer_data,
    GLenum bind_target
)
    : name{}, bind_target(bind_target), data_type(GL_FLOAT), attrib_size(3),
      data_size(sizeof(std::array<float, 3>) * buffer_data.size()), moved(false)
{
    glCreateBuffers(1, &name);
    glNamedBufferStorage(name, this->data_size, buffer_data.data(), 0);
}

template <>
StaticBuffer::StaticBuffer(
    std::vector<uint32_t> buffer_data,
    GLenum bind_target
)
    : name{}, bind_target(bind_target), data_type(GL_UNSIGNED_INT),
      attrib_size(1), data_size(sizeof(uint32_t) * buffer_data.size()),
      moved(false)
{
    glCreateBuffers(1, &name);
    glNamedBufferStorage(name, this->data_size, buffer_data.data(), 0);
}
