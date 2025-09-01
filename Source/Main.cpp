#include <GLFW/glfw3.h>
#include <GraphicsEngine.h>
#include <glm/gtc/matrix_transform.hpp>

void processEvents(const Input& input, const Timer& timer, Window& window, void* other = nullptr) {
    if (input.isKeyJustPressed(GLFW_KEY_ESCAPE))
        window.quit();
}

int main() {
    Window window = Window(800, 600, "Graphics Engine", true);
    // Scoped for cleanup - once the window context is destroyed, opengl calls will fail!
    {
        Camera camera = Camera(glm::vec3(0, 0, -3));
        Model model = Model("Resource/models/cube.gltf");
        Texture texture = Texture("Resource/textures/wall.jpg");
        Shader shader = Shader("Resource/shaders/3d_vert.glsl", "Resource/shaders/3d_frag.glsl", AS_FILE);
        Timer& timer = Timer::getInstance();
        Framebuffer framebuffer = Framebuffer(window.getWidth(), window.getHeight());
        while (window.isOpen()) {
            processEvents(Input::getInstance(), timer, window);
            framebuffer.bind();
            {
                Framebuffer::enableDepthTest();
                Framebuffer::setClearColor(.5, .5, .5);
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