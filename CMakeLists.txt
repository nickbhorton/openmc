cmake_minimum_required(VERSION 3.27)

project(openmc VERSION 0.0.1 LANGUAGES CXX C)

find_package(glm 1.0.1 REQUIRED)
find_package(glfw3 3.4 REQUIRED)

# get glad source code with glad python tool
execute_process(
    COMMAND glad 
    --api gl:core=4.5 --out-path ${CMAKE_BINARY_DIR}/glad
)
add_library(
    glad
    ${CMAKE_BINARY_DIR}/glad/src/gl.c
)
target_include_directories(
    glad
    SYSTEM PUBLIC 
    ${CMAKE_BINARY_DIR}/glad/include
)

add_executable(openmc src/entry.cpp 
    src/buffer.cpp src/buffer.h
    src/shader.h src/shader_object.cpp src/shader_program.cpp 
    src/vao.cpp src/vao.h
    src/texture.cpp src/texture.h
    src/geometry.cpp src/geometry.h
    src/chunk.cpp src/chunk.h
    src/image.cpp src/image.h
    src/face.cpp src/face.h
    src/block.cpp src/block.h
)
target_link_libraries(openmc PRIVATE glad glfw imgui)
target_compile_options(openmc PRIVATE -g)
