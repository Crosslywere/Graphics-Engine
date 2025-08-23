#include <Input.h>
#include <iostream>

Input::Input() {
}

Input& Input::getInstance() {
    static Input input;
    return input;
}

bool Input::isKeyPressed(int key) const {
    return m_keyset.find(key) != m_keyset.end() || m_prevKeyset.find(key) != m_prevKeyset.end();
}

bool Input::isKeyJustPressed(int key) const {
    return m_keyset.find(key) != m_keyset.end() && m_prevKeyset.find(key) == m_prevKeyset.end();
}

bool Input::isButtonPressed(int button) const {
    return m_bottonset.find(button) != m_bottonset.end();
}

const glm::vec2 &Input::getMousePosition() const {
    return m_mousePos;
}

void Input::update() {
    m_prevKeyset = m_keyset;
}

void Input::setKey(int key, bool pressed) {
    if (pressed)
        m_keyset.emplace(key);
    else
        m_keyset.erase(key);
}

void Input::setMouseButton(int button, bool pressed) {
    if (pressed)
        m_bottonset.emplace(button);
    else
        m_bottonset.erase(button);
}

void Input::setMousePos(float xpos, float ypos) {
    m_mousePos.x = xpos;
    m_mousePos.y = ypos;
}
