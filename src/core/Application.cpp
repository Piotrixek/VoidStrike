#include "Application.h"
#include <iostream>

Application::Application() : m_running(false) {
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    m_window = std::make_unique<Window>("VoidStrike - FPS", 1280, 720);
    if (!m_window->Initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return false;
    }

    m_renderer = std::make_unique<Renderer>();
    if (!m_renderer->Initialize(m_window->GetHandle(), m_window->GetWidth(), m_window->GetHeight())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }

    m_game = std::make_unique<Game>();
    if (!m_game->Initialize(m_renderer.get())) {
        std::cerr << "Failed to initialize game" << std::endl;
        return false;
    }

    m_lastTime = std::chrono::high_resolution_clock::now();
    m_running = true;

    return true;
}

void Application::Run() {
    while (m_running && !m_window->ShouldClose() && m_game->IsRunning()) {
        m_window->PollEvents();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
        m_lastTime = currentTime;

        deltaTime = (std::min)(deltaTime, 0.033f);

        Update(deltaTime);
        Render();
    }
}

void Application::Update(float deltaTime) {
    m_game->Update(deltaTime);
}

void Application::Render() {
    m_game->Render(m_renderer.get());
}

void Application::Shutdown() {
    m_game.reset();
    m_renderer.reset();
    m_window.reset();
}