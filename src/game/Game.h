#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "UI.h"
#include "graphics/Shader.h"
#include <vector>
#include <memory>

class Renderer;

class Game {
public:
    Game();
    ~Game();

    bool Initialize(Renderer* renderer);
    void Update(float deltaTime);
    void Render(Renderer* renderer);

    bool IsRunning() const { return m_isRunning; }
    void Quit() { m_isRunning = false; }

private:
    void SpawnEnemy();
    void CheckCollisions();
    void HandleShooting();
    void UpdateEnemies(float deltaTime);
    void CleanupDeadEnemies();

    Player m_player;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    Map m_map;
    UI m_ui;
    Shader m_shader;

    bool m_isRunning;
    bool m_initialized;
    float m_enemySpawnTimer;
    float m_enemySpawnInterval;
    int m_maxEnemies;

    Renderer* m_renderer;
};