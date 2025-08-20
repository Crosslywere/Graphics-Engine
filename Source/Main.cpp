#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <Texture.h>
#include <iostream>
#include <cmath>

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Grphics Engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create window!" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        std::cerr << "Failed to load OpenGL functions" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    // Scoped for cleanup - once the window context is destroyed, opengl calls will fail!
    {
        // Triangle setup
        float vertices[] = {
            0.0f, 0.5f, .5f, 1.f,
            -.5f, -.5f, 0.f, 0.f,
            0.5f, -.5f, 1.f, 0.f,
        };
        unsigned int vao, vbo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        Shader shader = Shader(
            R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec2 texCoord;
            varying vec2 uv;
            void main() {
                uv = texCoord;
                gl_Position = vec4(aPos, 0.0, 1.0);
            }
            )",
            R"(
            #version 330 core
            uniform vec3 color;
            uniform sampler2D wall;
            varying vec2 uv;
            void main() {
                vec3 w = texture2D(wall, uv).rgb;
                gl_FragColor = vec4(abs(color) * w, 1.0);
            }
            )"
        );
        shader.use();
        shader.setFloat3("color", 1, .5f, .25f);
        Texture texture = Texture("Resource/wall.jpg");
        shader.setTexture("wall", texture.bind());
        float r, g, b;
        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            r = sin(glfwGetTime() + M_PI * M_PI);
            g = sin(glfwGetTime());
            b = sin(glfwGetTime() * 2);
            shader.setFloat3("color", r, g, b);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}