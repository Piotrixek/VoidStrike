#pragma once
#include "graphics/Mesh.h"
#include "graphics/Shader.h"

class Renderer;

class UI {
public:
    UI();
    ~UI();

    void Initialize(Renderer* renderer);
    void Render(Renderer* renderer, float health, float maxHealth, int enemyCount);

private:
    void RenderHealthBar(Renderer* renderer, float health, float maxHealth);
    void RenderCrosshair(Renderer* renderer);
    void RenderEnemyCounter(Renderer* renderer, int enemyCount);

    Mesh m_healthBarBackgroundMesh;
    Mesh m_healthBarMesh;
    Mesh m_crosshairMesh;
    Shader m_uiShader;

    bool m_initialized;
};
