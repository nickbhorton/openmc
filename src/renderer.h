#pragma once

#include <array>

#include "vao.h"

int constexpr g_chunk_radius = 1;

struct Renderer {
    std::array<
        std::array<
            std::array<VertexArrayObject, g_chunk_radius * 2>,
            g_chunk_radius * 2>,
        g_chunk_radius * 2>
        vaos;
    std::array<
        std::array<std::array<size_t, g_chunk_radius * 2>, g_chunk_radius * 2>,
        g_chunk_radius * 2>
        draw_sizes;

    Renderer();
};
