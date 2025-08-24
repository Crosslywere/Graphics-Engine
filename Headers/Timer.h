#pragma once

class Timer {
public:
    static Timer& getInstance();
    float getTotalTime() const;
    float getDeltaTime() const;
    void update();
private:
    Timer() = default;
    float m_NowTime{};
    float m_PastTime{};
    float m_DeltaTime{};
};