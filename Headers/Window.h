#pragma once

#include <string>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const std::string& title, bool resizable = false, bool vsync = true);
    ~Window();
    bool isOpen() const;
    int getWidth() const;
    int getHeight() const;
    const std::string& getTitle() const;
    void setTitle(const std::string& title);
    bool isVsync() const;
    void setVsync(bool vsync);
private:
    int m_width, m_height;
    bool m_vsync, m_resizable;
    std::string m_title;
    GLFWwindow* m_handle;
};