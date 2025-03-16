#include <glad/gl.h>

#include <GLFW/glfw3.h>

// glm includes
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "block.h"
#include "chunk.h"
#include "image.h"
#include "renderer.h"
#include "texture.h"
#include "vao.h"
#include "window.h"
#include "world.h"

#include <array>
#include <cstdio>
#include <fstream>
#include <memory>

World g_world{};
std::unique_ptr<Renderer> g_renderer{};
std::unique_ptr<ShaderProgram> basic_s;
std::unique_ptr<StaticBuffer> g_face_position_geometry_b;

std::vector<std::array<float, 3>> const face_position_geometry{
    {{0, 0, 0}, {1, 0, 0}, {0, 0, 1}, {1, 0, 1}}
};

void update_renderer(Renderer& r, int cx, int cy, int cz)
{
    VertexArrayObject vao{};
    vao.attach_shader(*basic_s.get());
    vao.attach_buffer_object("v_position", *g_face_position_geometry_b.get());

    StaticBuffer faces_b(
        *g_world.get_chunk_mesh({cx, cy, cz}),
        GL_ARRAY_BUFFER
    );
    vao.attach_buffer_object("v_offset", std::move(faces_b), 1);

    r.vaos[cx + g_chunk_radius][cy + g_chunk_radius][cz + g_chunk_radius] =
        std::move(vao);
    r.draw_sizes[cx + g_chunk_radius][cy + g_chunk_radius]
                [cz + g_chunk_radius] = faces_b.byte_count() / sizeof(uint32_t);
}

void camera_update_per_frame(GLFWwindow* window)
{
    const float camera_speed = 0.25f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Window::camera_position +=
            camera_speed * glm::normalize(Window::camera_direction);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Window::camera_position -=
            camera_speed * glm::normalize(Window::camera_direction);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Window::camera_position -=
            glm::normalize(
                glm::cross(Window::camera_direction, Window::camera_up)
            ) *
            camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Window::camera_position +=
            glm::normalize(
                glm::cross(Window::camera_direction, Window::camera_up)
            ) *
            camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Window::camera_position += Window::camera_up * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        Window::camera_position -= Window::camera_up * camera_speed;
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    for (int cz = -g_chunk_radius; cz < g_chunk_radius; cz++) {
        for (int cy = -g_chunk_radius; cy < g_chunk_radius; cy++) {
            for (int cx = -g_chunk_radius; cx < g_chunk_radius; cx++) {
                if (cx == 0 && cy == 0 && cz == 0) {
                    g_world.generate_debug_chunk({cx, cy, cz});
                } else {
                    g_world.generate_chunk({cx, cy, cz});
                }
            }
        }
    }

    Window window{};

    // setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = NULL;

    ImGui_ImplGlfw_InitForOpenGL(window.get_glfw_window(), true);
    ImGui_ImplOpenGL3_Init();

    ImGuiWindowFlags window_flags_imgui{};
    window_flags_imgui |= ImGuiWindowFlags_NoMove;
    window_flags_imgui |= ImGuiWindowFlags_NoResize;
    window_flags_imgui |= ImGuiWindowFlags_NoCollapse;
    bool open_imgui{true};

    {
        std::vector<std::pair<std::string, GLenum>> basic_s_constructor_param{
            {{"../res/shaders/basic.vert.glsl", GL_VERTEX_SHADER},
             {"../res/shaders/basic.frag.glsl", GL_FRAGMENT_SHADER}}
        };
        basic_s = std::make_unique<ShaderProgram>(basic_s_constructor_param);

        ShaderProgram axis_s(
            {{"../res/shaders/axis.vert.glsl", GL_VERTEX_SHADER},
             {"../res/shaders/axis.frag.glsl", GL_FRAGMENT_SHADER}}
        );

        // loading the texture atlas
        std::string block_texture_dir =
            "/home/nick-dev/res/minecraft/textures/block/";

        std::vector<std::string> block_texture_filenames{};
        std::ifstream file("../res/texture_atlas.json");
        nlohmann::json filename_data = nlohmann::json::parse(file);
        for (auto const& v : filename_data) {
            block_texture_filenames.push_back(v.template get<std::string>());
        }

        std::vector<std::unique_ptr<Image>> images;
        for (auto const& path : block_texture_filenames) {
            images.push_back(std::make_unique<Image>(block_texture_dir + path));
        }

        std::vector<Image const*> images_to_stitch{};
        for (auto const& img : images) {
            images_to_stitch.push_back(img.get());
        }

        Image stiched{images_to_stitch, g_texture_square_width};

        Texture texture_atlas{stiched, 0};

        g_renderer = std::make_unique<Renderer>();

        g_face_position_geometry_b = std::make_unique<StaticBuffer>(
            face_position_geometry,
            GL_ARRAY_BUFFER
        );

        for (int cz = -g_chunk_radius; cz < g_chunk_radius; cz++) {
            for (int cy = -g_chunk_radius; cy < g_chunk_radius; cy++) {
                for (int cx = -g_chunk_radius; cx < g_chunk_radius; cx++) {
                    update_renderer(*g_renderer.get(), cx, cy, cz);
                }
            }
        }

        std::vector<std::array<float, 3>> axis_positions{{
            {0, 0, 0},
        }};
        std::vector<std::array<float, 3>> axis_colors{{
            {1, 0, 0},
        }};
        StaticBuffer axis_positions_b(axis_positions, GL_ARRAY_BUFFER);
        StaticBuffer axis_colors_b(axis_colors, GL_ARRAY_BUFFER);

        VertexArrayObject axis_vao{};
        axis_vao.attach_shader(axis_s);
        axis_vao.attach_buffer_object("v_position", axis_positions_b);
        axis_vao.attach_buffer_object("v_color", axis_colors_b);

        // imgui vars
        bool imgui_wireframe{false};
        bool imgui_vsync{true};
        glPointSize(10);

        while (!glfwWindowShouldClose(window.get_glfw_window())) {
            glfwPollEvents();
            if (Window::mouse_capture) {
                camera_update_per_frame(window.get_glfw_window());
            }
            if (imgui_wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            if (imgui_vsync) {
                glfwSwapInterval(1);
            } else {
                glfwSwapInterval(0);
            }

            Window::camera_direction.x = std::cos(glm::radians(Window::yaw)) *
                                         std::cos(glm::radians(Window::pitch));
            Window::camera_direction.y = sin(glm::radians(Window::pitch));
            Window::camera_direction.z = std::sin(glm::radians(Window::yaw)) *
                                         std::cos(glm::radians(Window::pitch));
            glm::mat4 view = glm::lookAt(
                Window::camera_position,
                Window::camera_position +
                    glm::normalize(Window::camera_direction),
                Window::camera_up
            );
            glm::mat4 proj = glm::perspective(
                glm::radians(45.0f),
                static_cast<float>(Window::width) /
                    static_cast<float>(Window::height),
                0.1f,
                1000.0f
            );
            basic_s->update_uniform("view", view);
            basic_s->update_uniform("proj", proj);
            axis_s.update_uniform("view", glm::identity<glm::mat4>());
            axis_s.update_uniform("proj", glm::identity<glm::mat4>());

            // clear screen white
            glm::vec4 constexpr bg_color{1, 1, 1, 1};
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));

            // draw
            for (int cz = -g_chunk_radius; cz < g_chunk_radius; cz++) {
                for (int cy = -g_chunk_radius; cy < g_chunk_radius; cy++) {
                    for (int cx = -g_chunk_radius; cx < g_chunk_radius; cx++) {
                        g_renderer
                            ->vaos[cx + g_chunk_radius][cy + g_chunk_radius]
                                  [cz + g_chunk_radius]
                            .bind();
                        basic_s->update_uniform(
                            "chunk_offset",
                            glm::vec3(
                                static_cast<float>(cx) * g_chunk_size,
                                static_cast<float>(cy) * g_chunk_size,
                                static_cast<float>(cz) * g_chunk_size
                            )
                        );
                        glDrawArraysInstanced(
                            GL_TRIANGLE_STRIP,
                            0,
                            static_cast<GLsizei>(face_position_geometry.size()),
                            g_renderer->draw_sizes[cx + g_chunk_radius]
                                                  [cy + g_chunk_radius]
                                                  [cz + g_chunk_radius]
                        );
                    }
                }
            }

            axis_vao.bind();
            glDrawArrays(
                GL_POINTS,
                0,
                static_cast<GLsizei>(axis_positions.size())
            );

            // draw the imgui window
            if (!Window::mouse_capture) {
                float constexpr imgui_window_padding{10};
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                ImGui::Begin("openmc", &open_imgui, window_flags_imgui);
                ImGui::SetWindowPos(
                    ImVec2(imgui_window_padding, imgui_window_padding)
                );
                ImGui::SetWindowSize(ImVec2(
                    Window::width - 2 * imgui_window_padding,
                    Window::height - 2 * imgui_window_padding
                ));
                ImGui::DragFloat("Yaw", &Window::yaw, Window::yaw);
                ImGui::DragFloat("Pitch", &Window::pitch, Window::pitch);
                ImGui::InputFloat3(
                    "Camera Position",
                    glm::value_ptr(Window::camera_position)
                );
                ImGui::Checkbox("Wireframe", &imgui_wireframe);
                ImGui::Checkbox("Vsync", &imgui_vsync);
                ImGui::Text("%d triangles rendered", 0);
                ImGui::Text("%d vao drawn", 0);
                ImGui::Text("%.2f FPS", io.Framerate);
                ImGui::End();

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }

            glfwSwapBuffers(window.get_glfw_window());
        }
    }
}
