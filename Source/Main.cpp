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
#include <Mesh.h>

void processInputs(Window& window) {
    static auto& input = Input::getInstance();
    if (input.isKeyJustPressed(GLFW_KEY_ESCAPE))
        window.quit();
    if (input.isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        auto rg = input.getMousePosition();
        glClearColor(rg.r / window.getWidth(), rg.g / window.getHeight(), sin(glfwGetTime()) * .5 + .5, 1);
    } else {
        glClearColor(0, 0, 0, 1);
    }
}

int main() {
    Window window = Window(800, 600, "Graphics Engine", true);
    // Scoped for cleanup - once the window context is destroyed, opengl calls will fail!
    {
        // Rectangle setup
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };
        Mesh rectangle = Mesh(indices, 6,
            {{2, {
                -.5f, 0.5f,
                -.5f, -.5f,
                0.5f, -.5f,
                0.5f, 0.5f,
            }}, {2, {
                0.f, 1.f,
                0.f, 0.f,
                1.f, 0.f,
                1.f, 1.f, 
            }}}
        );
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
            processInputs(window);
            glClear(GL_COLOR_BUFFER_BIT);
            shader.setMat4("projection", camera.getProjection(window.getWidth(), window.getHeight()));
            if (!Input::getInstance().isKeyPressed(' '))
                shader.setMat4("model", glm::rotate(glm::mat4(1.f), (float)glfwGetTime(), glm::vec3(0, 0, 1)));
            r = sin(glfwGetTime() + M_PI * M_PI);
            g = sin(glfwGetTime());
            b = sin(glfwGetTime() * 2);
            shader.setFloat3("color", r, g, b);
            rectangle.draw();
        }
    }
}