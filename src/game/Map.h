#pragma once
#include "math/Vector3.h"
#include "math/Matrix4.h"
#include "graphics/Mesh.h"
#include <vector>

class Renderer;

struct Wall {
    Vector3 position;
    Vector3 scale;
    Vector3 color;
};

class Map {
public:
    Map();
    ~Map();

    void Initialize(Renderer* renderer);
    void Render(Renderer* renderer, const Matrix4& view, const Matrix4& projection);

    bool CheckCollision(const Vector3& position, float radius) const;
    Vector3 GetSpawnPoint() const { return m_spawnPoint; }

    const std::vector<Vector3>& GetEnemySpawnPoints() const { return m_enemySpawnPoints; }

private:
    void CreateBasicMap();

    std::vector<Wall> m_walls;
    std::vector<Wall> m_floor;
    std::vector<Wall> m_roof;
    Vector3 m_spawnPoint;
    std::vector<Vector3> m_enemySpawnPoints;

    Mesh m_wallMesh;
    Mesh m_floorMesh;
    Mesh m_roofMesh;
    bool m_meshesInitialized;
};