 
#pragma once
#include "math/Vector3.h"
#include "math/Matrix4.h"

class Camera {
public:
    Camera(const Vector3& position = Vector3(0.0f, 0.0f, 0.0f));
    
    void SetPosition(const Vector3& position) { m_position = position; }
    void SetRotation(float yaw, float pitch) { m_yaw = yaw; m_pitch = pitch; UpdateVectors(); }
    
    const Vector3& GetPosition() const { return m_position; }
    const Vector3& GetFront() const { return m_front; }
    const Vector3& GetUp() const { return m_up; }
    const Vector3& GetRight() const { return m_right; }
    
    float GetYaw() const { return m_yaw; }
    float GetPitch() const { return m_pitch; }
    
    Matrix4 GetViewMatrix() const;
    Matrix4 GetProjectionMatrix(float aspect) const;
    
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessKeyboard(const Vector3& direction, float deltaTime);

private:
    void UpdateVectors();
    
    Vector3 m_position;
    Vector3 m_front;
    Vector3 m_up;
    Vector3 m_right;
    Vector3 m_worldUp;
    
    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_fov;
};