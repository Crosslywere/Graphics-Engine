#include <glad/glad.h>
#include <Window.h>
#include <iostream>
#include <Input.h>

Window::Window(int width, int height, const std::string& title, bool resizable, bool vsync)
    : m_width(width), m_height(height), m_title(title), m_resizable(resizable), m_vsync(vsync) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, m_resizable);
    m_handle = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
    if (!m_handle) {
        glfwTerminate();
        std::cerr << "Failed to create window!" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(m_handle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(m_handle);
        glfwTerminate();
        std::cerr << "Failed to load OpenGL functions" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(m_vsync);
    glfwSetWindowSizeLimits(m_handle, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(m_handle, this);
    glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow* window, int width, int height) {
        auto& windowReference = *(Window*)glfwGetWindowUserPointer(window);
        windowReference.m_width = width;
        windowReference.m_height = height;
        glViewport(0, 0, width, height);
    });
    glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Input::getInstance().setKey(key, action);
    });
    glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int scancode, int action) {
        Input::getInstance().setMouseButton(button, action);
    });
    glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double xpos, double ypos) {
        Input::getInstance().setMousePos(xpos, ypos);
    });
}

Window::~Window() {
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

bool Window::isOpen() const {
    Input::getInstance().update();
    glfwSwapBuffers(m_handle);
    glfwPollEvents();
    return !glfwWindowShouldClose(m_handle);
}

int Window::getWidth() const {
    return m_width;
}

int Window::getHeight() const {
    return m_height;
}

const std::string &Window::getTitle() const
{
    return m_title;
}

void Window::setTitle(const std::string &title) {
    m_title = title;
}

bool Window::isVsync() const {
    return m_vsync;
}

void Window::setVsync(bool vsync) {
    glfwSwapInterval(m_vsync = vsync);
}
