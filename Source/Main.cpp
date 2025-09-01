#include <GLFW/glfw3.h>
#include <GraphicsEngine.h>
#include <glm/gtc/matrix_transform.hpp>

void processEvents(const Input& input, const Timer& timer, Window& window, void* other = nullptr) {
    if (input.isKeyJustPressed(GLFW_KEY_ESCAPE))
        window.quit();
    auto camera = (Camera*)other;
    if (input.isKeyPressed(GLFW_KEY_SPACE))
        camera->setPosition(camera->getPosition() + (camera->getWorldUp() * timer.getDeltaTime()));
    if (input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        camera->setPosition(camera->getPosition() - (camera->getWorldUp() * timer.getDeltaTime()));
    if (input.isKeyPressed(GLFW_KEY_W))
        camera->setPosition(camera->getPosition() + (camera->getFront() * timer.getDeltaTime()));
    if (input.isKeyPressed(GLFW_KEY_S))
        camera->setPosition(camera->getPosition() - (camera->getFront() * timer.getDeltaTime()));
    if (input.isKeyPressed(GLFW_KEY_D))
        camera->setPosition(camera->getPosition() + (camera->getRight() * timer.getDeltaTime()));
    if (input.isKeyPressed(GLFW_KEY_A))
        camera->setPosition(camera->getPosition() - (camera->getRight() * timer.getDeltaTime()));
    if (input.isKeyPressed(GLFW_KEY_E))
        camera->setYaw(camera->getYaw() + 50 * timer.getDeltaTime());
    if (input.isKeyPressed(GLFW_KEY_Q))
        camera->setYaw(camera->getYaw() - 50 * timer.getDeltaTime());
}

int main() {
    Window window = Window(800, 600, "Graphics Engine", true);
    // Scoped for cleanup - once the window context is destroyed, opengl calls will fail!
    {
        Camera camera = Camera(glm::vec3(0, 0, -3));
        Model model = Model("Resource/models/cube.glb");
        Texture texture = Texture("Resource/textures/wall.jpg");
        Shader shader = Shader("Resource/shaders/3d_vert.glsl", "Resource/shaders/3d_frag.glsl", AS_FILE);
        Timer& timer = Timer::getInstance();
        Framebuffer framebuffer = Framebuffer(window.getWidth(), window.getHeight());
        Framebuffer::enableDepthTest();
        Framebuffer::setClearColor(.5, .5, .5);
        while (window.isOpen()) {
            processEvents(Input::getInstance(), timer, window, &camera);
            framebuffer.bind();
            {
                Framebuffer::clear();
                shader.use();
                shader.setTexture("uTexture", texture.bind());
                shader.setMat4("uProjection", camera.getProjection(window.getWidth(), window.getHeight()));
                shader.setMat4("uView", camera.getView());
                shader.setMat4("uModel", glm::rotate(glm::mat4(1.f), timer.getTotalTime(), glm::normalize(glm::vec3(1, 1, -1))));
                model.draw(shader);
            }
            framebuffer.drawToScreen();
        }
    }
}