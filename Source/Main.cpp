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
#include <Timer.h>

static inline Camera g_Camera = Camera({0, 0, -1});

void processInputs(Window& window) {
    static const auto& input = Input::getInstance();
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
        Mesh rectangle = Mesh(
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
            }}},
            {0, 1, 2, 2, 3, 0}
        );
        // Creating the shader
        Shader shader = Shader("Resource/shaders/vertex.glsl", "Resource/shaders/fragment.glsl", AS_FILE);
        shader.use();

        // Setting up the texture
        Texture texture = Texture("Resource/wall.jpg");
        shader.setTexture("wall", texture.bind());
        glm::vec3 color;
        while (window.isOpen()) {
            const auto time = Timer::getInstance().getTotalTime();
            processInputs(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            shader.setMat4("projection", g_Camera.getProjection(window.getWidth(), window.getHeight()));
            shader.setMat4("view", g_Camera.getView());
            if (!Input::getInstance().isKeyPressed(' '))
                shader.setMat4("model", glm::rotate(glm::mat4(1.f), (float)glfwGetTime(), glm::vec3(0, 0, 1)));
            color = glm::vec3(sin(-time), sin(time), cos(time));
            shader.setFloat3("color", color.r, color.g, color.b);
            rectangle.draw();
        }
    }
}