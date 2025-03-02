#pragma once

#include "buffer.h"
#include "shader.h"

#include <glad/gl.h>

#include <iostream>

#define BUFFER_OFFSET(offset) ((void*)(offset))

class VertexArrayObject
{
    GLuint name;
    GLuint shader_name;
    bool moved;

public:
    VertexArrayObject();
    ~VertexArrayObject();

    VertexArrayObject(VertexArrayObject const& other) = delete;
    VertexArrayObject& operator=(VertexArrayObject const& other) = delete;
    VertexArrayObject(VertexArrayObject&& other) noexcept;
    VertexArrayObject& operator=(VertexArrayObject&& other) noexcept;

    template <typename T>
    void attach_buffer_object(
        StaticBuffer<T>& buffer,
        GLuint index,
        GLint dimentions,
        GLenum type,
        GLboolean normalized,
        GLsizei stride
    );
    template <typename T>
    void attach_buffer_object(
        std::string const& attribute_name,
        StaticBuffer<T>& buffer,
        GLuint divisor = 0
    );

    void attach_shader(ShaderProgram const& shader);

    template <typename T> void attach_element_array(StaticBuffer<T>& buffer);

    void bind();
};

template <typename T>
void VertexArrayObject::attach_buffer_object(
    std::string const& attribute_name,
    StaticBuffer<T>& buffer,
    GLuint divisor
)
{
    std::cerr << "voa buffer type not supported\n";
}

template <typename T>
void VertexArrayObject::attach_buffer_object(
    StaticBuffer<T>& buffer,
    GLuint index,
    GLint dimentions,
    GLenum type,
    GLboolean normalized,
    GLsizei stride
)
{
    glBindVertexArray(name);
    buffer.bind();
    glVertexAttribPointer(
        index,           // index
        dimentions,      // number of numbers
        type,            // type
        normalized,      // normalized
        stride,          // stride aka how tightly packed are the numbers
        BUFFER_OFFSET(0) // how far into the buffer is the fisrt num
    );
    glEnableVertexAttribArray(index);
}

template <typename T>
void VertexArrayObject::attach_element_array(StaticBuffer<T>& buffer)
{
    glBindVertexArray(name);
    buffer.bind();
}
