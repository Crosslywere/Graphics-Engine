cmake_minimum_required(VERSION 3.10)

project(Graphics_Engine VERSION 1.0)

add_subdirectory(Vendor/glfw)

add_executable(Graphics_Engine
    Vendor/glad/src/glad.c
    Source/Main.cpp
    Source/Shader.cpp
)

target_include_directories(Graphics_Engine PRIVATE
    Vendor/glad/include
    Vendor/glfw/include
    Headers
)

target_link_libraries(Graphics_Engine PRIVATE
    glfw
)