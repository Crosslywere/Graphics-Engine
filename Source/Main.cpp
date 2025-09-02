#include <GLFW/glfw3.h>
#include <GraphicsEngine.h>
#include <glm/gtc/matrix_transform.hpp>

void processEvents(Window& window, Camera& camera) {
    static const Input& input = Input::getInstance();
    static const Timer& timer = Timer::getInstance();
    if (input.isKeyJustPressed(GLFW_KEY_ESCAPE))
        window.quit();
    float speed = input.isKeyPressed(GLFW_KEY_LEFT_SHIFT) ? 2.5 : 1;
    if (input.isKeyPressed(GLFW_KEY_SPACE))
        camera.setPosition(camera.getPosition() + camera.getWorldUp() * timer.getDeltaTime() * speed);
    if (input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        camera.setPosition(camera.getPosition() - camera.getWorldUp() * timer.getDeltaTime() * speed);
    if (input.isKeyPressed(GLFW_KEY_W))
        camera.setPosition(camera.getPosition() + camera.getFront() * timer.getDeltaTime() * speed);
    if (input.isKeyPressed(GLFW_KEY_S))
        camera.setPosition(camera.getPosition() - camera.getFront() * timer.getDeltaTime() * speed);
    if (input.isKeyPressed(GLFW_KEY_D))
        camera.setPosition(camera.getPosition() + camera.getRight() * timer.getDeltaTime() * speed);
    if (input.isKeyPressed(GLFW_KEY_A))
        camera.setPosition(camera.getPosition() - camera.getRight() * timer.getDeltaTime() * speed);
    if (input.isKeyPressed(GLFW_KEY_E))
        camera.setYaw(camera.getYaw() + 50 * timer.getDeltaTime());
    if (input.isKeyPressed(GLFW_KEY_Q))
        camera.setYaw(camera.getYaw() - 50 * timer.getDeltaTime());
    if (input.isKeyPressed(GLFW_KEY_L))
        camera.setYaw(camera.getYaw() + 50 * timer.getDeltaTime());
    if (input.isKeyPressed(GLFW_KEY_J))
        camera.setYaw(camera.getYaw() - 50 * timer.getDeltaTime());
    if (input.isKeyPressed(GLFW_KEY_I))
        camera.setPitch(camera.getPitch() + 50 * timer.getDeltaTime());
    if (input.isKeyPressed(GLFW_KEY_K))
        camera.setPitch(camera.getPitch() - 50 * timer.getDeltaTime());

    static glm::vec2 lastPos = input.getMousePosition();
    if (input.isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
        window.disableMouse();
        auto mousePos = input.getMousePosition();
        float xoffset = mousePos.x - lastPos.x;
        float yoffset = lastPos.y - mousePos.y;
        lastPos = mousePos;
        camera.setPitch(camera.getPitch() + yoffset * 10 * timer.getDeltaTime());
        camera.setYaw(camera.getYaw() + xoffset * 10 * timer.getDeltaTime());
    } else {
        lastPos = input.getMousePosition();
        window.normalMouse();
    }
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
        Framebuffer::enableCullFace();
        Framebuffer::setClearColor(.5, .5, .5);
        while (window.isOpen()) {
            processEvents(window, camera);
            framebuffer.bind();
            {
                Framebuffer::clear();
                shader.use();
                shader.setTexture("uTexture", texture);
                shader.setMat4("uProjection", camera.getProjection((float) window.getWidth() / window.getHeight()));
                shader.setMat4("uView", camera.getView());
                shader.setMat4("uModel", glm::rotate(glm::mat4(1.f), timer.getTotalTime(), glm::vec3(1, 1, -1)));
                model.draw(shader);
            }
            framebuffer.drawToScreen();
        }
    }
}