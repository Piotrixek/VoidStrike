#include "Camera.h"
#include <algorithm>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 1.0f;
const float FOV = 45.0f;
const float PI = 3.14159265359f;

Camera::Camera(const Vector3& position)
    : m_position(position)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(YAW)
    , m_pitch(PITCH)
    , m_movementSpeed(SPEED)
    , m_mouseSensitivity(SENSITIVITY)
    , m_fov(FOV) {
    UpdateVectors();
}

Matrix4 Camera::GetViewMatrix() const {
    return Matrix4::lookAt(m_position, m_position + m_front, m_up);
}

Matrix4 Camera::GetProjectionMatrix(float aspect) const {
    return Matrix4::perspective(m_fov * PI / 180.0f, aspect, 0.1f, 100.0f);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw -= xoffset;
    m_pitch -= yoffset;

    if (constrainPitch) {
        m_pitch = (std::max)(-89.0f, (std::min)(89.0f, m_pitch));
    }

    UpdateVectors();
}

void Camera::ProcessKeyboard(const Vector3& direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    m_position += direction * velocity;
}

void Camera::UpdateVectors() {

    Vector3 front;
    front.x = std::cos(m_yaw * PI / 180.0f) * std::cos(m_pitch * PI / 180.0f);
    front.y = std::sin(m_pitch * PI / 180.0f);
    front.z = std::sin(m_yaw * PI / 180.0f) * std::cos(m_pitch * PI / 180.0f);
    m_front = front.normalize();

    m_right = m_worldUp.cross(m_front).normalize();
    m_up = m_front.cross(m_right).normalize();
}
