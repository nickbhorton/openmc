#include "buffer.h"
#include "shader.h"
#include "vao.h"
#include "window.h"

#include <glad/gl.h>

#include <array>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec4 constexpr background_color{1, 1, 1, 1};

int main()
{
    Window window{};
    ShaderProgram basic_s{
        {{"../res/shaders/axis.vert.glsl", GL_VERTEX_SHADER},
         {"../res/shaders/axis.frag.glsl", GL_FRAGMENT_SHADER}}
    };

    std::vector<std::array<float, 3>> positions{};
    positions.push_back({-0.5, -0.5, 0});
    positions.push_back({+0.0, +0.5, 0});
    positions.push_back({+0.5, -0.5, 0});
    StaticBuffer positions_b{positions, GL_ARRAY_BUFFER};

    std::vector<std::array<float, 3>> colors{};
    colors.push_back({1, 0, 0});
    colors.push_back({0, 1, 0});
    colors.push_back({0, 0, 1});
    StaticBuffer colors_b{colors, GL_ARRAY_BUFFER};

    VertexArrayObject vao;
    vao.attach_shader(basic_s);
    vao.attach_buffer_object("v_position", positions_b);
    vao.attach_buffer_object("v_color", colors_b);

    while (!glfwWindowShouldClose(window.get_glfw_window())) {
        glfwPollEvents();
        if (Window::mouse_capture) {
            camera_update_per_frame(window.get_glfw_window());
            basic_s.update_uniform("view", Window::view);
            basic_s.update_uniform("proj", Window::proj);
        }

        // clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(background_color));

        // draw
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, positions.size());

        // swap buffers
        glfwSwapBuffers(window.get_glfw_window());
    }
}
