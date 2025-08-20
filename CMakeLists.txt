cmake_minimum_required(VERSION 3.10)

project(Graphics_Engine VERSION 1.0)

add_subdirectory(Vendor/glfw)

add_executable(Graphics_Engine
    Vendor/glad/src/glad.c
    Source/Main.cpp
    Source/Shader.cpp
    Source/stb_implementation.c
    Source/Texture.cpp
)

target_include_directories(Graphics_Engine PRIVATE
    Vendor/glad/include
    Vendor/glfw/include
    Headers
    Vendor/stb
)

target_link_libraries(Graphics_Engine PRIVATE
    glfw
)

target_compile_definitions(Graphics_Engine PRIVATE
    ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)