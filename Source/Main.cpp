#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <Texture.h>
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <Camera.h>
#include <Window.h>
#include <Input.h>

int main() {
    Window window = Window(800, 600, "Graphics Engine", true);
    // Scoped for cleanup - once the window context is destroyed, opengl calls will fail!
    {
        // Rectangle setup
        float vertices[] = {
            -.5f, 0.5f, 0.f, 1.f,
            -.5f, -.5f, 0.f, 0.f,
            0.5f, -.5f, 1.f, 0.f,
            0.5f, 0.5f, 1.f, 1.f,
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };
        unsigned int vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Creating the shader
        Shader shader = Shader("Resource/shaders/vertex.glsl", "Resource/shaders/fragment.glsl", AS_FILE);
        shader.use();

        // Setting up the camera
        Camera camera = Camera({0, 0, -1});
        shader.setMat4("view", camera.getView());
        shader.setMat4("model", glm::mat4(1));

        // Setting up the texture
        Texture texture = Texture("Resource/wall.jpg");
        shader.setTexture("wall", texture.bind());
        float r, g, b;
        while (window.isOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
            shader.setMat4("projection", camera.getProjection(window.getWidth(), window.getHeight()));
            if (Input::getInstance().isKeyPressed(GLFW_KEY_SPACE))
                shader.setMat4("model", glm::rotate(glm::mat4(1.f), (float)glfwGetTime(), glm::vec3(0, 0, 1)));
            r = sin(glfwGetTime() + M_PI * M_PI);
            g = sin(glfwGetTime());
            b = sin(glfwGetTime() * 2);
            shader.setFloat3("color", r, g, b);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        }
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}