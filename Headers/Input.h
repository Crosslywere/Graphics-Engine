#pragma once

#include <unordered_set>
#include <glm/glm.hpp>

class Input {
    friend class Window;
public:
    Input(Input&) = delete;
    Input(Input&&)= delete;
    static Input& getInstance();
    bool isKeyPressed(int key) const;
    bool isKeyJustPressed(int key) const;
    bool isKeyJustReleased(int key) const;
    bool isButtonPressed(int button) const;
    bool isButtonJustPressed(int button) const;
    bool isButtonJustReleased(int button) const;
    const glm::vec2& getMousePosition() const;
private:
    Input();
    void update();
    void setKey(int key, bool pressed);
    void setMouseButton(int button, bool pressed);
    void setMousePos(float xpos, float ypos);
private:
    std::unordered_set<int> m_keyset{};
    std::unordered_set<int> m_prevKeyset{};
    std::unordered_set<int> m_buttonset{};
    std::unordered_set<int> m_prevButtonset{};
    glm::vec2 m_mousePos{};

};