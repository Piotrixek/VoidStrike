#include "UI.h"
#include "graphics/Renderer.h"
#include <vector>

UI::UI() : m_initialized(false) {
}

UI::~UI() {
}

void UI::Initialize(Renderer* renderer) {
    if (m_initialized) return;

    m_uiShader.LoadVertexShader(renderer->GetDevice(), L"shaders/ui_vertex.hlsl");
    m_uiShader.LoadPixelShader(renderer->GetDevice(), L"shaders/ui_pixel.hlsl");

    ComPtr<ID3D11InputLayout> uiInputLayout = renderer->CreateInputLayoutForShader(&m_uiShader);
    if (uiInputLayout) {
        m_uiShader.SetInputLayout(uiInputLayout);
    }

    // Initialize Health Bar Background
    std::vector<Vertex> backgroundVertices = {
        {{-0.9f, -0.9f, 0.0f}, {0.2f, 0.2f, 0.2f}},
        {{-0.5f, -0.9f, 0.0f}, {0.2f, 0.2f, 0.2f}},
        {{-0.5f, -0.85f, 0.0f}, {0.2f, 0.2f, 0.2f}},
        {{-0.9f, -0.85f, 0.0f}, {0.2f, 0.2f, 0.2f}}
    };
    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
    m_healthBarBackgroundMesh.Initialize(renderer->GetDevice(), backgroundVertices, indices);

    // Initialize Crosshair
    std::vector<Vertex> crosshairVertices = {
        // Horizontal bar
        {{-0.02f, -0.003f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.02f, -0.003f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.02f,  0.003f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{-0.02f,  0.003f, 0.0f}, {1.0f, 1.0f, 1.0f}},

        // Vertical bar
        {{-0.003f, -0.03f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.003f, -0.03f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.003f,  0.03f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{-0.003f,  0.03f, 0.0f}, {1.0f, 1.0f, 1.0f}}
    };
    std::vector<unsigned int> crosshairIndices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };
    m_crosshairMesh.Initialize(renderer->GetDevice(), crosshairVertices, crosshairIndices);


    m_initialized = true;
}

void UI::Render(Renderer* renderer, float health, float maxHealth, int enemyCount) {
    if (!m_initialized) return;

    ID3D11DeviceContext* context = renderer->GetContext();

    // Disable depth testing for UI
    context->OMSetDepthStencilState(nullptr, 0);

    m_uiShader.Bind(context);

    // Set an identity matrix for UI elements as they are in screen space
    Matrix4 identity;
    renderer->SetMatrices(identity, identity, identity);
    renderer->UpdateConstantBuffer();

    RenderHealthBar(renderer, health, maxHealth);
    RenderCrosshair(renderer);
    RenderEnemyCounter(renderer, enemyCount);

    // Re-enable depth testing for the rest of the scene if needed (good practice)
    renderer->SetDefaultDepthStencilState();
}

void UI::RenderHealthBar(Renderer* renderer, float health, float maxHealth) {
    // Render the background bar
    m_healthBarBackgroundMesh.Render(renderer->GetContext());

    float healthPercentage = (maxHealth > 0.0f) ? (health / maxHealth) : 0.0f;

    if (healthPercentage > 0.0f) {
        float barWidth = 0.4f * healthPercentage;
        Vector3 color = { 1.0f - healthPercentage, healthPercentage, 0.0f };

        std::vector<Vertex> healthVertices = {
            {{-0.9f, -0.9f, 0.0f}, color},
            {{-0.9f + barWidth, -0.9f, 0.0f}, color},
            {{-0.9f + barWidth, -0.85f, 0.0f}, color},
            {{-0.9f, -0.85f, 0.0f}, color}
        };
        std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

        // Re-initialize the health bar mesh with new vertices
        m_healthBarMesh.Initialize(renderer->GetDevice(), healthVertices, indices);
        m_healthBarMesh.Render(renderer->GetContext());
    }
}

void UI::RenderCrosshair(Renderer* renderer) {
    m_crosshairMesh.Render(renderer->GetContext());
}

void UI::RenderEnemyCounter(Renderer* renderer, int enemyCount) {
    // This part is not implemented, but you could render text here
    (void)renderer;
    (void)enemyCount;
}
