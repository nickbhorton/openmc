#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

void camera_update_per_frame(GLFWwindow* window);

class Window
{
public:
    Window();
    ~Window();

    static GLFWwindow* create_glfw_window();
    GLFWwindow* get_glfw_window();

    static void glfw_error_callback(int error, const char* desc);
    static void glfw_key_callback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    );
    static void glfw_window_resize_callback(
        GLFWwindow* window,
        int new_width,
        int new_height
    );
    static void
    glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos);

    // screen static variables
    static int width;
    static int height;
    static GLFWwindow* glfw_window;

    // mouse static variables
    static bool mouse_capture;
    static bool first_mouse_update;
    static int mouse_last_x;
    static int mouse_last_y;

    // camera static variables
    static float pitch;
    static float yaw;

    static glm::vec3 camera_position;
    static glm::vec3 camera_up;
    static glm::vec3 camera_direction;
    static glm::mat4 view;
    static glm::mat4 proj;
};
