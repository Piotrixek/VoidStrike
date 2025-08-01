#include "Game.h"
#include "graphics/Renderer.h"
#include "input/InputManager.h"
#include <algorithm>
#include <random>

Game::Game()
    : m_player(Vector3(0.0f, 1.5f, 0.0f))
    , m_isRunning(true)
    , m_initialized(false)
    , m_enemySpawnTimer(0.0f)
    , m_enemySpawnInterval(2.5f)
    , m_maxEnemies(12)
    , m_renderer(nullptr) {
}

Game::~Game() {
}

bool Game::Initialize(Renderer* renderer) {
    if (m_initialized) return true;

    m_renderer = renderer;

    if (!m_shader.LoadVertexShader(renderer->GetDevice(), L"shaders/vertex.hlsl")) {
        return false;
    }

    if (!m_shader.LoadPixelShader(renderer->GetDevice(), L"shaders/pixel.hlsl")) {
        return false;
    }

    ComPtr<ID3D11InputLayout> inputLayout = renderer->CreateInputLayoutForShader(&m_shader);
    if (!inputLayout) {
        return false;
    }
    m_shader.SetInputLayout(inputLayout);

    m_map.Initialize(renderer);
    m_ui.Initialize(renderer);

    InputManager::Instance().SetCursorLocked(true);
    InputManager::Instance().SetMouseSensitivity(0.1f);

    SpawnEnemy();

    m_initialized = true;
    return true;
}

void Game::Update(float deltaTime) {
    if (!m_initialized) return;

    InputManager& input = InputManager::Instance();
    input.Update();

    if (input.IsKeyPressed(VK_ESCAPE)) {
        m_isRunning = false;
        return;
    }

    m_player.Update(deltaTime);

    UpdateEnemies(deltaTime);
    CheckCollisions();
    HandleShooting();
    CleanupDeadEnemies();

    m_enemySpawnTimer += deltaTime;
    if (m_enemySpawnTimer >= m_enemySpawnInterval && m_enemies.size() < m_maxEnemies) {
        SpawnEnemy();
        m_enemySpawnTimer = 0.0f;
    }
}

void Game::Render(Renderer* renderer) {
    if (!m_initialized) return;

    renderer->Clear(0.2f, 0.3f, 0.6f, 1.0f);

    m_shader.Bind(renderer->GetContext());

    Matrix4 view = m_player.GetCamera().GetViewMatrix();
    Matrix4 projection = m_player.GetCamera().GetProjectionMatrix(
        static_cast<float>(renderer->GetWidth()) / static_cast<float>(renderer->GetHeight())
    );

    m_map.Render(renderer, view, projection);

    for (auto& enemy : m_enemies) {
        enemy->Render(renderer, view, projection);
    }

    m_ui.Render(renderer, m_player.GetHealth(), 100.0f, static_cast<int>(m_enemies.size()));

    renderer->Present();
}

void Game::SpawnEnemy() {
    const auto& spawnPoints = m_map.GetEnemySpawnPoints();
    if (spawnPoints.empty()) return;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(spawnPoints.size() - 1));

    Vector3 spawnPos = spawnPoints[dis(gen)];

    std::uniform_real_distribution<float> colorDis(0.5f, 1.0f);
    Vector3 enemyColor(colorDis(gen), 0.1f, 0.1f);

    m_enemies.push_back(std::make_unique<Enemy>(spawnPos, enemyColor));
}

void Game::UpdateEnemies(float deltaTime) {
    for (auto& enemy : m_enemies) {
        if (enemy->IsAlive()) {
            enemy->Update(deltaTime, m_player.GetPosition());
        }
    }
}

void Game::CheckCollisions() {
    Vector3 playerPos = m_player.GetPosition();
    float playerRadius = 0.5f;

    for (auto& enemy : m_enemies) {
        if (!enemy->IsAlive()) continue;

        Vector3 enemyPos = enemy->GetPosition();
        float distance = (playerPos - enemyPos).length();

        if (distance < (playerRadius + enemy->GetSize() * 0.5f)) {
            m_player.TakeDamage(enemy->GetDamage() * 0.016f);

            if (!m_player.IsAlive()) {
                m_isRunning = false;
            }
        }
    }
}

void Game::HandleShooting() {
    InputManager& input = InputManager::Instance();

    if (input.IsKeyPressed(VK_LBUTTON)) {
        Vector3 rayOrigin = m_player.GetCamera().GetPosition();
        Vector3 rayDirection = m_player.GetCamera().GetFront();

        Enemy* closestEnemy = nullptr;
        float closestDistance = 100.0f;

        for (auto& enemy : m_enemies) {
            if (!enemy->IsAlive()) continue;

            Vector3 toEnemy = enemy->GetPosition() - rayOrigin;
            float t = toEnemy.dot(rayDirection);

            if (t > 0) {
                Vector3 projection = rayOrigin + rayDirection * t;
                float distanceToRay = (enemy->GetPosition() - projection).length();

                if (distanceToRay < enemy->GetSize() * 0.5f) {
                    float distance = toEnemy.length();
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestEnemy = enemy.get();
                    }
                }
            }
        }

        if (closestEnemy) {
            closestEnemy->TakeDamage(40.0f);
        }
    }
}

void Game::CleanupDeadEnemies() {
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return !enemy->IsAlive();
            }),
        m_enemies.end()
    );
}
