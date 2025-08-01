#include "Player.h"
#include "input/InputManager.h"
#include <algorithm>

Player::Player(const Vector3& startPosition)
    : m_camera(startPosition)
    , m_position(startPosition)
    , m_health(100.0f)
    , m_maxHealth(100.0f)
    , m_speed(6.0f) {
}

void Player::Update(float deltaTime) {
    InputManager& input = InputManager::Instance();

    float mouseX, mouseY;
    input.GetMouseDelta(mouseX, mouseY);
    m_camera.ProcessMouseMovement(mouseX, mouseY);

    Vector3 movement(0.0f, 0.0f, 0.0f);

    Vector3 forward = m_camera.GetFront();
    forward.y = 0;
    forward.normalize();

    Vector3 right = m_camera.GetRight();

    if (input.IsKeyDown('W')) {
        movement += forward;
    }
    if (input.IsKeyDown('S')) {
        movement = movement - forward;
    }
    if (input.IsKeyDown('A')) {
        movement = movement - right;
    }
    if (input.IsKeyDown('D')) {
        movement += right;
    }

    if (movement.length() > 0.0f) {
        movement = movement.normalize();

        Vector3 currentPosition = m_camera.GetPosition();
        currentPosition += movement * m_speed * deltaTime;
        m_camera.SetPosition(currentPosition);
    }

    // to prevent flying e im locking the player at a constant height and the gravity/collision system would be a future improvement
    Vector3 finalPosition = m_camera.GetPosition();
    finalPosition.y = 1.5f;
    m_camera.SetPosition(finalPosition);

    m_position = m_camera.GetPosition();
}

void Player::TakeDamage(float damage) {
    m_health = (std::max)(0.0f, m_health - damage);
}
