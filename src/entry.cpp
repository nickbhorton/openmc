#include <glad/gl.h>

#include <GLFW/glfw3.h>

// glm includes
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "chunk.h"
#include "texture.h"
#include "vao.h"

#include <array>
#include <cstdio>
#include <iostream>
#include <limits>

typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

typedef glm::ivec2 ivec2;

typedef glm::mat4 mat4;

ivec2 g_window_size(200, 200);
ivec2 g_last{g_window_size[0] / 2, g_window_size[1] / 2};
float g_yaw{-90.0};
float g_pitch{0.0};
vec3 g_camera_position = vec3(0.0, 0.0, 3.0);
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
    int scancode,
    int action,
    int mods
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

void glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
    // std::cout << "window resized to " << width << "x" << height << std::endl;
    g_window_size.x = width;
    g_window_size.y = height;
    glViewport(0, 0, width, height);
}

GLFWwindow* create_glfw_window(vec2 window_size)
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

void glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (first_mouse_update) {
        g_last[0] = xpos;
        g_last[1] = ypos;
        first_mouse_update = false;
    }

    if (mouse_capture) {
        float xoffset = xpos - g_last[0];
        // reversed since y-coordinates range from bottom to top
        float yoffset = g_last[1] - ypos;
        g_last[0] = xpos;
        g_last[1] = ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        g_yaw += xoffset;
        g_pitch += yoffset;
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

int main(int argc, char* argv[])
{
    glfwInit();
    glfwSetErrorCallback(glfw_error_callback);

    GLFWwindow* window = create_glfw_window(g_window_size);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetFramebufferSizeCallback(window, glfw_window_resize_callback);
    glfwSetCursorPosCallback(window, glfw_mouse_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
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

        Texture texture{
            "/home/nick-dev/res/minecraft/textures/block/stone.png",
            0
        };

        Chunk test_chunk{};
        for (size_t i = 0; i < test_chunk.block_mask.size(); i++) {
            test_chunk.block_mask[i] = std::numeric_limits<uint64_t>::max();
        }

        std::vector<uint32_t> offsets{};
        for (uint32_t x = 0; x < g_chunk_size; x++) {
            for (uint32_t z = 0; z < g_chunk_size; z++) {
                for (uint32_t y = 0; y < g_chunk_size; y++) {
                    if (test_chunk.test_block_mask(x, y, z)) {
                        offsets.push_back((0 << 18) | (z << 12) | (y << 6) | x);
                        offsets.push_back((1 << 18) | (z << 12) | (y << 6) | x);
                        offsets.push_back((2 << 18) | (z << 12) | (y << 6) | x);
                        offsets.push_back((3 << 18) | (z << 12) | (y << 6) | x);
                        offsets.push_back((4 << 18) | (z << 12) | (y << 6) | x);
                        offsets.push_back((5 << 18) | (z << 12) | (y << 6) | x);
                    }
                }
            }
        }

        StaticBuffer offsets_b(offsets, GL_ARRAY_BUFFER);
        std::cout << offsets.size() << "\n";

        std::vector<std::array<float, 3>> positions{
            {{0, 0, 0}, {1, 0, 0}, {0, 0, 1}, {1, 0, 1}}
        };
        std::vector<std::array<float, 2>> uvs{{{0, 0}, {1, 0}, {0, 1}, {1, 1}}};

        StaticBuffer positions_b(positions, GL_ARRAY_BUFFER);
        StaticBuffer uvs_b(uvs, GL_ARRAY_BUFFER);

        VertexArrayObject vao{};
        vao.attach_shader(basic_s);
        vao.attach_buffer_object("v_position", positions_b);
        vao.attach_buffer_object("v_uv", uvs_b);
        vao.attach_buffer_object("v_offset", offsets_b, 1);

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
                cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
            camera_direction.y = sin(glm::radians(g_pitch));
            camera_direction.z =
                sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
            mat4 view = glm::lookAt(
                g_camera_position,
                g_camera_position + glm::normalize(camera_direction),
                camera_up
            );
            mat4 proj = glm::perspective(
                glm::radians(45.0f),
                static_cast<float>(g_window_size[0]) / g_window_size[1],
                0.1f,
                1000.0f
            );
            basic_s.update_uniform_mat4f("view", view);
            basic_s.update_uniform_mat4f("proj", proj);

            // clear screen white
            vec4 constexpr bg_color{1, 1, 1, 1};
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));

            // draw
            vao.bind();
            glDrawArraysInstanced(
                GL_TRIANGLE_STRIP,
                0,
                positions.size(),
                offsets.size()
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
