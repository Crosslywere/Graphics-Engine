#pragma once

#include <glm/glm.hpp>

class Camera {
    friend void updateVectors(Camera&);
public:
    Camera(const glm::vec3& position = glm::vec3(0, 1, -1), float pitch = 0, float yaw = 90, float fov = 60);
    const glm::mat4 getProjection(float width, float height) const;
    const glm::mat4 getView() const;
    static const glm::vec3& getWorldUp();
    const glm::vec3& getPosition() const;
    void setPosition(const glm::vec3& position);
    float getPitch() const;
    void setPitch(float pitch);
    float getYaw() const;
    void setYaw(float yaw);
    float getFovY() const;
    void setFovY(float fovy);
    const glm::vec3& getFront() const;
    const glm::vec3& getRight() const;
    const glm::vec3& getUp() const;
private:
    static const glm::vec3 WORLD_UP;
    glm::vec3 m_position, m_front, m_right, m_up;
    float m_pitch, m_yaw, m_fovy;
};