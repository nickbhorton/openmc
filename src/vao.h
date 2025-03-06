#pragma once

#include "buffer.h"
#include "shader.h"

#include <glad/gl.h>

#include <iostream>

#define BUFFER_OFFSET(offset) ((void*)(offset))

class VertexArrayObject
{
public:
    VertexArrayObject();

    VertexArrayObject(VertexArrayObject const& other) = delete;
    VertexArrayObject& operator=(VertexArrayObject const& other) = delete;
    VertexArrayObject(VertexArrayObject&& other) noexcept;
    VertexArrayObject& operator=(VertexArrayObject&& other) noexcept;

    ~VertexArrayObject();

    void bind();
    void attach_shader(ShaderProgram const& shader);

    template <typename T>
    void attach_buffer_object(
        std::string const& attribute_name,
        StaticBuffer<T>& buffer,
        GLuint divisor = 0
    );

private:
    GLuint name;
    GLuint shader_name;
    bool moved;
};

template <typename T>
void VertexArrayObject::attach_buffer_object(
    std::string const& attribute_name,
    StaticBuffer<T>& buffer,
    GLuint divisor
)
{
    std::cerr << "vao buffer type not implemented\n";
}
