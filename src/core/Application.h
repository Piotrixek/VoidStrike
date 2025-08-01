 
#pragma once
#include "Window.h"
#include "graphics/Renderer.h"
#include "game/Game.h"
#include <memory>
#include <chrono>

class Application {
public:
    Application();
    ~Application();
    
    bool Initialize();
    void Run();
    void Shutdown();

private:
    void Update(float deltaTime);
    void Render();
    
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Game> m_game;
    
    std::chrono::high_resolution_clock::time_point m_lastTime;
    bool m_running;
};