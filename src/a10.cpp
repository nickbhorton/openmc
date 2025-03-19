#include "buffer.h"
#include "shader.h"
#include "vao.h"
#include "window.h"

#include <glad/gl.h>

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
        {{"../res/shaders/basic.vert.glsl", GL_VERTEX_SHADER},
         {"../res/shaders/basic.frag.glsl", GL_FRAGMENT_SHADER}}
    };

    ShaderProgram compute_s{
        {{"../res/shaders/basic.comp.glsl", GL_COMPUTE_SHADER}}
    };

    size_t constexpr x_discritization{1000};
    size_t constexpr z_discritization{1000};
    compute_s.update_uniform("xd", static_cast<GLuint>(x_discritization));
    compute_s.update_uniform("zd", static_cast<GLuint>(z_discritization));
    std::vector<glm::vec4> partical_positions{
        x_discritization * z_discritization,
        glm::vec4(0, 0, 0, 1)
    };

    GLuint partical_positions_ssbo{};
    glCreateBuffers(1, &partical_positions_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partical_positions_ssbo);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        static_cast<int>(partical_positions.size()) *
            sizeof(partical_positions[0]),
        partical_positions.data(),
        GL_DYNAMIC_DRAW
    );

    VertexArrayObject vao{};
    vao.bind();
    glBindBuffer(GL_ARRAY_BUFFER, partical_positions_ssbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

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

        // update partical positions with compute shader
        compute_s.bind();
        glDispatchCompute(x_discritization, z_discritization, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        // render the triangles
        basic_s.bind();
        vao.bind();
        glDrawArrays(GL_POINTS, 0, partical_positions.size());

        // swap buffers
        glfwSwapBuffers(window.get_glfw_window());

        compute_s.update_uniform("t", static_cast<float>(glfwGetTime()));
    }
}
