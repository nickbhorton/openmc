#include <glad/gl.h>

#include <GLFW/glfw3.h>

// glm includes
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <nlohmann/json.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "block.h"
#include "chunk.h"
#include "image.h"
#include "texture.h"
#include "vao.h"
#include "world.h"

#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json_fwd.hpp>

typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

typedef glm::ivec2 ivec2;

typedef glm::uvec3 uvec3;
typedef glm::uvec2 uvec2;

typedef glm::mat4 mat4;

ivec2 g_window_size(200, 200);
ivec2 g_last{g_window_size[0] / 2, g_window_size[1] / 2};
float g_yaw{45.0};
float g_pitch{11.5};
vec3 g_camera_position = vec3(-3.0, -1.0, -3.0);
vec3 camera_up = vec3(0.0, 1.0, 0.0);
vec3 camera_direction = vec3(
    cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch)),
    sin(glm::radians(g_pitch)),
    sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch))
);
bool mouse_capture{};
bool first_mouse_update{true};

static void glfw_error_callback(int error, const char* desc)
{
    std::cerr << "glfw_error_callback " << error << " " << desc << std::endl;
    std::exit(1);
}

static void glfw_key_callback(
    GLFWwindow* window,
    int key,
    [[maybe_unused]] int scancode,
    int action,
    [[maybe_unused]] int mods
)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (!mouse_capture) {
            mouse_capture = true;
            first_mouse_update = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            mouse_capture = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void glfw_window_resize_callback(
    [[maybe_unused]] GLFWwindow* window,
    int width,
    int height
)
{
    // std::cout << "window resized to " << width << "x" << height << std::endl;
    g_window_size.x = width;
    g_window_size.y = height;
    glViewport(0, 0, width, height);
}

GLFWwindow* create_glfw_window(ivec2 window_size)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        window_size[0],
        window_size[1],
        "ngi window",
        NULL,
        NULL
    );
    if (!window) {
        glfw_error_callback(-1, "window was null");
    }

    glfwMakeContextCurrent(window);

    int glad_version = gladLoadGL(glfwGetProcAddress);
    if (glad_version == 0) {
        glfw_error_callback(-1, "glad verison what 0");
    }

    return window;
}

void glfw_mouse_callback(
    [[maybe_unused]] GLFWwindow* window,
    double xpos,
    double ypos
)
{
    if (first_mouse_update) {
        g_last[0] = static_cast<int>(xpos);
        g_last[1] = static_cast<int>(ypos);
        first_mouse_update = false;
    }

    if (mouse_capture) {
        double xoffset = xpos - static_cast<double>(g_last[0]);
        // reversed since y-coordinates range from bottom to top
        double yoffset = static_cast<double>(g_last[1]) - ypos;
        g_last[0] = static_cast<int>(xpos);
        g_last[1] = static_cast<int>(ypos);

        const double sensitivity = 0.1;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        g_yaw += static_cast<float>(xoffset);
        g_pitch += static_cast<float>(yoffset);
        if (g_pitch > 89.0f) {
            g_pitch = 89.0f;
        }
        if (g_pitch < -89.0f) {
            g_pitch = -89.0f;
        }
    }
}

void frame_input(GLFWwindow* window)
{
    const float camera_speed = 0.25f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_camera_position += camera_speed * glm::normalize(camera_direction);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_camera_position -= camera_speed * glm::normalize(camera_direction);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_camera_position -=
            glm::normalize(glm::cross(camera_direction, camera_up)) *
            camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_camera_position +=
            glm::normalize(glm::cross(camera_direction, camera_up)) *
            camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        g_camera_position += camera_up * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        g_camera_position -= camera_up * camera_speed;
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    glfwInit();
    glfwSetErrorCallback(glfw_error_callback);

    GLFWwindow* window = create_glfw_window(g_window_size);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetFramebufferSizeCallback(window, glfw_window_resize_callback);
    glfwSetCursorPosCallback(window, glfw_mouse_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glViewport(0, 0, g_window_size[0], g_window_size[1]);

    mouse_capture = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = NULL;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    ImGuiWindowFlags window_flags_imgui{};
    window_flags_imgui |= ImGuiWindowFlags_NoMove;
    window_flags_imgui |= ImGuiWindowFlags_NoResize;
    window_flags_imgui |= ImGuiWindowFlags_NoCollapse;
    bool open_imgui{true};

    {
        ShaderProgram basic_s(
            {{"../res/shaders/basic.vert.glsl", GL_VERTEX_SHADER},
             {"../res/shaders/basic.frag.glsl", GL_FRAGMENT_SHADER}}
        );

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
        std::cout << block_texture_filenames.size() << "\n";

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

        std::vector<std::array<float, 3>> face_position_geometry{
            {{0, 0, 0}, {1, 0, 0}, {0, 0, 1}, {1, 0, 1}}
        };
        StaticBuffer face_position_geometry_b(
            face_position_geometry,
            GL_ARRAY_BUFFER
        );

        World world{};

        ivec2 chunk_count = ivec2(1, 1);
        for (int cy = -chunk_count.x; cy < chunk_count.y; cy++) {
            for (int cx = -chunk_count.x; cx < chunk_count.x; cx++) {
                if (cx == 0 && cy == 0) {
                    world.generate_debug_chunk({cx, 0, cy});
                } else {
                    world.generate_chunk({cx, 0, cy});
                }
            }
        }

        std::vector<VertexArrayObject> chunk_vaos{};
        std::vector<size_t> chunk_face_counts{};
        std::vector<vec2> chunk_position{};
        size_t vao_count{0};
        for (int cy = -chunk_count.x; cy < chunk_count.y; cy++) {
            for (int cx = -chunk_count.x; cx < chunk_count.x; cx++) {
                VertexArrayObject vao{};

                auto faces(world.get_chunk_mesh({cx, 0, cy}));
                StaticBuffer faces_b(faces, GL_ARRAY_BUFFER);

                vao.attach_shader(basic_s);
                vao.attach_buffer_object(
                    "v_position",
                    face_position_geometry_b
                );
                vao.attach_buffer_object("v_offset", std::move(faces_b), 1);
                chunk_vaos.push_back(std::move(vao));
                chunk_face_counts.push_back(faces.size());
                chunk_position.push_back(
                    {static_cast<float>(cx) * g_chunk_size,
                     static_cast<float>(cy) * g_chunk_size}
                );
                vao_count++;
            }
        }

        std::vector<std::array<float, 3>> axis_positions{{
            {0, 0, 0},
            {1, 0, 0},
            {0, 0, 0},
            {0, 1, 0},
            {0, 0, 0},
            {0, 0, 1},
        }};
        std::vector<std::array<float, 3>> axis_colors{{
            {1, 0, 0},
            {1, 0, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 0, 1},
            {0, 0, 1},
        }};
        StaticBuffer axis_positions_b(axis_positions, GL_ARRAY_BUFFER);
        StaticBuffer axis_colors_b(axis_colors, GL_ARRAY_BUFFER);

        VertexArrayObject axis_vao{};
        axis_vao.attach_shader(axis_s);
        axis_vao.attach_buffer_object("v_position", axis_positions_b);
        axis_vao.attach_buffer_object("v_color", axis_colors_b);

        std::cout << vao_count << "\n";

        // imgui vars
        bool imgui_wireframe{false};
        bool imgui_vsync{true};

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            if (mouse_capture) {
                frame_input(window);
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

            camera_direction.x =
                std::cos(glm::radians(g_yaw)) * std::cos(glm::radians(g_pitch));
            camera_direction.y = sin(glm::radians(g_pitch));
            camera_direction.z =
                std::sin(glm::radians(g_yaw)) * std::cos(glm::radians(g_pitch));
            mat4 view = glm::lookAt(
                g_camera_position,
                g_camera_position + glm::normalize(camera_direction),
                camera_up
            );
            mat4 proj = glm::perspective(
                glm::radians(45.0f),
                static_cast<float>(g_window_size[0]) /
                    static_cast<float>(g_window_size[1]),
                0.1f,
                1000.0f
            );
            basic_s.update_uniform("view", view);
            basic_s.update_uniform("proj", proj);
            axis_s.update_uniform("view", view);
            axis_s.update_uniform("proj", proj);

            // clear screen white
            vec4 constexpr bg_color{1, 1, 1, 1};
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));

            // draw
            for (size_t c = 0; c < vao_count; c++) {
                chunk_vaos[c].bind();
                basic_s.update_uniform("chunk_offset", chunk_position[c]);
                glDrawArraysInstanced(
                    GL_TRIANGLE_STRIP,
                    0,
                    static_cast<GLsizei>(face_position_geometry.size()),
                    chunk_face_counts[c]
                );
            }

            axis_vao.bind();
            glDrawArrays(
                GL_LINES,
                0,
                static_cast<GLsizei>(axis_positions.size())
            );

            // draw the imgui window
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("openmc", &open_imgui, window_flags_imgui);
            ImGui::SetWindowPos(ImVec2(0, 0));
            ImGui::SetWindowSize(ImVec2(400, 200));
            ImGui::DragFloat("Yaw", &g_yaw, g_yaw);
            ImGui::DragFloat("Pitch", &g_pitch, g_pitch);
            ImGui::InputFloat3(
                "Camera Position",
                glm::value_ptr(g_camera_position)
            );
            ImGui::Checkbox("Wireframe", &imgui_wireframe);
            ImGui::Checkbox("Vsync", &imgui_vsync);
            ImGui::Text("%.2f FPS", io.Framerate);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
