#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GraphicsEngine.h>
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

void processEvents(const Input& input, const Timer& timer, Window& window, void* other) {
    if (input.isKeyJustPressed(GLFW_KEY_ESCAPE))
        window.quit();
    if (input.isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        window.disableMouse();
        auto rg = input.getMousePosition();
        Framebuffer::setClearColor(rg.r / window.getWidth(), rg.g / window.getHeight(), sin(timer.getTotalTime() * M_PI) * .5 + .5);
    } else {
        window.normalMouse();
        Framebuffer::setClearColor(0, 0, 0);
    }
}

int main() {
    Window window = Window(800, 600, "Graphics Engine", true);
    // Scoped for cleanup - once the window context is destroyed, opengl calls will fail!
    {
        Camera camera = Camera(glm::vec3(0, 0, -3));
        Model model = Model("Resource/models/cube.gltf");
        Texture texture = Texture("Resource/textures/wall.jpg");
        Shader shader = Shader("Resource/shaders/3d_vert.glsl", "Resource/shaders/3d_frag.glsl", AS_FILE);
        shader.setTexture("uTexture", texture.bind());
        Timer& timer = Timer::getInstance();
        while (window.isOpen()) {
            processEvents(Input::getInstance(), timer, window, &camera);
            Framebuffer::clear();
            shader.setMat4("uProjection", camera.getProjection(window.getWidth(), window.getHeight()));
            shader.setMat4("uView", camera.getView());
            shader.setMat4("uModel", glm::rotate(glm::mat4(1.f), timer.getTotalTime(), glm::normalize(glm::vec3(1, 1, -1))));
            model.draw(shader);
        }
    }
}