#include <Camera.h>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 Camera::WORLD_UP = glm::vec3(0, 1, 0);

Camera::Camera(const glm::vec3& position, float pitch, float yaw, float fov)
    : m_position(position), m_pitch(pitch), m_yaw(yaw), m_fovy(fov) {
    updateVectors(*this);
}

const glm::mat4 Camera::getProjection(float aspectRatio) const {
    return glm::perspective(glm::radians(m_fovy), aspectRatio, m_near, m_far);
}

const glm::mat4 Camera::getProjection(float width, float height) const {
    return glm::ortho(-width / 2, width / 2, -height / 2, height / 2, m_near, m_far);
}

const glm::mat4 Camera::getView() const {
    return glm::lookAt(m_position, m_position + m_front, WORLD_UP);
}

const glm::vec3& Camera::getWorldUp() {
    return WORLD_UP;
}

const glm::vec3& Camera::getPosition() const {
    return m_position;
}

void Camera::setPosition(const glm::vec3 &position) {
    m_position = position;
}

float Camera::getPitch() const {
    return m_pitch;
}

void Camera::setPitch(float pitch) {
    m_pitch = pitch;
    updateVectors(*this);
}

float Camera::getYaw() const {
    return m_yaw;
}

void Camera::setYaw(float yaw) {
    m_yaw = yaw;
    updateVectors(*this);
}

float Camera::getFovY() const {
    return m_fovy;
}

void Camera::setFovY(float fovy) {
    m_fovy = fovy;
}

void Camera::setNearDistance(float near) {
    m_near = near;
}

void Camera::setFarDistance(float far) {
    m_far = far;
}

const glm::vec3& Camera::getFront() const {
    return m_front;
}

const glm::vec3& Camera::getRight() const {
    return m_right;
}

const glm::vec3& Camera::getUp() const {
    return m_up;
}

void updateVectors(Camera& camera) {
    glm::vec3 front{};
    front.x = cos(glm::radians(camera.getYaw())) * cos(glm::radians(camera.getPitch()));
    front.y = sin(glm::radians(camera.getPitch()));
    front.z = sin(glm::radians(camera.getYaw())) * cos(glm::radians(camera.getPitch()));
    camera.m_front = glm::normalize(front);
    camera.m_right = glm::normalize(glm::cross(front, camera.getWorldUp()));
    camera.m_up = glm::normalize(glm::cross(camera.getRight(), camera.getFront()));
}
