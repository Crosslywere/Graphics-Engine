#include <Timer.h>
#include <GLFW/glfw3.h>

Timer& Timer::getInstance() {
    static Timer timer;
    return timer;
}

float Timer::getTotalTime() const {
    return glfwGetTime();
}

float Timer::getDeltaTime() const {
    return m_DeltaTime;
}

void Timer::update() {
    m_NowTime = glfwGetTime();
    m_DeltaTime = m_NowTime - m_PastTime;
    m_PastTime = m_NowTime;
}