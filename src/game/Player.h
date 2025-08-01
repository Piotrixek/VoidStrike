 
#pragma once
#include "graphics/Camera.h"
#include "math/Vector3.h"

class Player {
public:
    Player(const Vector3& startPosition = Vector3(0.0f, 0.0f, 0.0f));
    
    void Update(float deltaTime);
    
    Camera& GetCamera() { return m_camera; }
    const Vector3& GetPosition() const { return m_position; }
    
    void SetHealth(float health) { m_health = health; }
    float GetHealth() const { return m_health; }
    void TakeDamage(float damage);
    
    bool IsAlive() const { return m_health > 0.0f; }

private:
    Camera m_camera;
    Vector3 m_position;
    float m_health;
    float m_maxHealth;
    float m_speed;
};