#include "Map.h"
#include "graphics/Renderer.h"

Map::Map() : m_spawnPoint(0.0f, 1.0f, 0.0f), m_meshesInitialized(false) {
    CreateBasicMap();
}

Map::~Map() {
}

void Map::CreateBasicMap() {
    Vector3 wallColor(0.8f, 0.8f, 0.8f);
    Vector3 floorColor(0.2f, 0.7f, 0.2f);
    Vector3 roofColor(0.1f, 0.1f, 0.1f);

    m_walls.push_back({ {-10.0f, 2.0f, -10.0f}, {1.0f, 4.0f, 20.0f}, wallColor });
    m_walls.push_back({ {10.0f, 2.0f, -10.0f}, {1.0f, 4.0f, 20.0f}, wallColor });
    m_walls.push_back({ {0.0f, 2.0f, -10.0f}, {20.0f, 4.0f, 1.0f}, wallColor });
    m_walls.push_back({ {0.0f, 2.0f, 10.0f}, {20.0f, 4.0f, 1.0f}, wallColor });

    m_walls.push_back({ {-5.0f, 2.0f, -5.0f}, {2.0f, 4.0f, 2.0f}, wallColor });
    m_walls.push_back({ {5.0f, 2.0f, 5.0f}, {2.0f, 4.0f, 2.0f}, wallColor });
    m_walls.push_back({ {-3.0f, 2.0f, 7.0f}, {3.0f, 4.0f, 1.0f}, wallColor });

    m_floor.push_back({ {0.0f, 0.0f, 0.0f}, {20.0f, 0.1f, 20.0f}, floorColor });
    m_floor.push_back({ {0.0f, 6.0f, 0.0f}, {20.0f, 0.1f, 20.0f}, roofColor });

    m_enemySpawnPoints.push_back(Vector3(-8.0f, 1.0f, -8.0f));
    m_enemySpawnPoints.push_back(Vector3(8.0f, 1.0f, -8.0f));
    m_enemySpawnPoints.push_back(Vector3(-8.0f, 1.0f, 8.0f));
    m_enemySpawnPoints.push_back(Vector3(8.0f, 1.0f, 8.0f));
    m_enemySpawnPoints.push_back(Vector3(0.0f, 1.0f, -8.0f));
    m_enemySpawnPoints.push_back(Vector3(0.0f, 1.0f, 8.0f));
    m_enemySpawnPoints.push_back(Vector3(-6.0f, 1.0f, 2.0f));
    m_enemySpawnPoints.push_back(Vector3(6.0f, 1.0f, -2.0f));
}

void Map::Initialize(Renderer* renderer) {
    if (!m_meshesInitialized) {
        m_wallMesh = Mesh::CreateCube(renderer->GetDevice(), Vector3(0.8f, 0.8f, 0.8f));
        m_floorMesh = Mesh::CreateCube(renderer->GetDevice(), Vector3(0.2f, 0.7f, 0.2f));
        m_roofMesh = Mesh::CreateCube(renderer->GetDevice(), Vector3(0.1f, 0.1f, 0.1f));
        m_meshesInitialized = true;
    }
}

void Map::Render(Renderer* renderer, const Matrix4& view, const Matrix4& projection) {
    if (!m_meshesInitialized) return;

    for (const auto& wall : m_walls) {
        Matrix4 world = Matrix4::translate(wall.position) * Matrix4::scale(wall.scale);
        renderer->SetMatrices(world, view, projection);
        renderer->UpdateConstantBuffer();
        m_wallMesh.Render(renderer->GetContext());
    }

    for (const auto& floor : m_floor) {
        Matrix4 world = Matrix4::translate(floor.position) * Matrix4::scale(floor.scale);
        renderer->SetMatrices(world, view, projection);
        renderer->UpdateConstantBuffer();

        if (floor.position.y > 3.0f) {
            m_roofMesh.Render(renderer->GetContext());
        }
        else {
            m_floorMesh.Render(renderer->GetContext());
        }
    }
}

bool Map::CheckCollision(const Vector3& position, float radius) const {
    for (const auto& wall : m_walls) {
        Vector3 minBounds = wall.position - wall.scale * 0.5f;
        Vector3 maxBounds = wall.position + wall.scale * 0.5f;

        if (position.x + radius > minBounds.x && position.x - radius < maxBounds.x &&
            position.y + radius > minBounds.y && position.y - radius < maxBounds.y &&
            position.z + radius > minBounds.z && position.z - radius < maxBounds.z) {
            return true;
        }
    }
    return false;
}