#include "Enemy.h"
#include "graphics/Renderer.h"
#include <algorithm>

Enemy::Enemy(const Vector3& position, const Vector3& color)
    : m_position(position)
    , m_color(color)
    , m_velocity(0.0f, 0.0f, 0.0f)
    , m_health(75.0f)
    , m_speed(2.5f)
    , m_damage(15.0f)
    , m_size(1.2f)
    , m_meshInitialized(false) {
}

void Enemy::Update(float deltaTime, const Vector3& playerPosition) {
    if (!IsAlive()) return;

    Vector3 direction = (playerPosition - m_position).normalize();
    m_velocity = direction * m_speed;
    m_position += m_velocity * deltaTime;
}

void Enemy::Render(Renderer* renderer, const Matrix4& view, const Matrix4& projection) {
    if (!IsAlive()) return;

    if (!m_meshInitialized) {
        m_mesh = Mesh::CreateCube(renderer->GetDevice(), m_color);
        m_meshInitialized = true;
    }

    Matrix4 world = Matrix4::translate(m_position) * Matrix4::scale(Vector3(m_size, m_size, m_size));
    renderer->SetMatrices(world, view, projection);
    renderer->UpdateConstantBuffer();

    m_mesh.Render(renderer->GetContext());
}

void Enemy::TakeDamage(float damage) {
    m_health = (std::max)(0.0f, m_health - damage);
}