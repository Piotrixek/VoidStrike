 
#pragma once
#include <windows.h>
#include <unordered_map>

class InputManager {
public:
    static InputManager& Instance();
    
    void Update();
    bool IsKeyPressed(int key) const;
    bool IsKeyDown(int key) const;
    bool IsKeyReleased(int key) const;
    
    void GetMouseDelta(float& deltaX, float& deltaY);
    void SetMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
    void SetCursorLocked(bool locked);

private:
    InputManager() = default;
    
    std::unordered_map<int, bool> m_currentKeys;
    std::unordered_map<int, bool> m_previousKeys;
    
    POINT m_lastMousePos;
    float m_mouseDeltaX = 0.0f;
    float m_mouseDeltaY = 0.0f;
    float m_mouseSensitivity = 1.0f;
    bool m_cursorLocked = false;
    bool m_firstMouse = true;
};