cmake_minimum_required(VERSION 3.28)

project(Graphics_Engine VERSION 1.0)

add_subdirectory(Vendor/assimp)
add_subdirectory(Vendor/glfw)

add_executable(Graphics_Engine
    Vendor/glad/src/glad.c
    Source/Camera.cpp
    Source/Framebuffer.cpp
    Source/Input.cpp
    Source/Main.cpp
    Source/Mesh.cpp
    Source/Model.cpp
    Source/Shader.cpp
    Source/stb_implementation.c
    Source/Texture.cpp
    Source/Window.cpp
    Source/Timer.cpp
    Source/utils.cpp
)

target_include_directories(Graphics_Engine PRIVATE
    Headers
    Vendor/assimp/include
    Vendor/glad/include
    Vendor/glfw/include
    Vendor/glm
    Vendor/stb
)

target_link_libraries(Graphics_Engine PRIVATE
    assimp
    glfw
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_compile_definitions(Graphics_Engine PRIVATE
    ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)