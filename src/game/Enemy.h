#pragma once
#include "math/Vector3.h"
#include "math/Matrix4.h"
#include "graphics/Mesh.h"

class Renderer;

class Enemy {
public:
    Enemy(const Vector3& position, const Vector3& color = Vector3(1.0f, 0.0f, 0.0f));

    void Update(float deltaTime, const Vector3& playerPosition);
    void Render(Renderer* renderer, const Matrix4& view, const Matrix4& projection);

    const Vector3& GetPosition() const { return m_position; }
    bool IsAlive() const { return m_health > 0.0f; }
    void TakeDamage(float damage);

    float GetDamage() const { return m_damage; }
    float GetSize() const { return m_size; }

private:
    Vector3 m_position;
    Vector3 m_color;
    Vector3 m_velocity;
    float m_health;
    float m_speed;
    float m_damage;
    float m_size;
    Mesh m_mesh;
    bool m_meshInitialized;
};