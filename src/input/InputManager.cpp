 
#include "InputManager.h"

InputManager& InputManager::Instance() {
    static InputManager instance;
    return instance;
}

void InputManager::Update() {
    m_previousKeys = m_currentKeys;
    
    m_currentKeys[VK_ESCAPE] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
    m_currentKeys['W'] = GetAsyncKeyState('W') & 0x8000;
    m_currentKeys['A'] = GetAsyncKeyState('A') & 0x8000;
    m_currentKeys['S'] = GetAsyncKeyState('S') & 0x8000;
    m_currentKeys['D'] = GetAsyncKeyState('D') & 0x8000;
    m_currentKeys[VK_SPACE] = GetAsyncKeyState(VK_SPACE) & 0x8000;
    m_currentKeys[VK_LSHIFT] = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
    m_currentKeys[VK_LBUTTON] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    
    if (m_cursorLocked) {
        POINT currentPos;
        GetCursorPos(&currentPos);
        
        if (m_firstMouse) {
            m_lastMousePos = currentPos;
            m_firstMouse = false;
        }
        
        m_mouseDeltaX = (currentPos.x - m_lastMousePos.x) * m_mouseSensitivity;
        m_mouseDeltaY = (currentPos.y - m_lastMousePos.y) * m_mouseSensitivity;
        
        RECT rect;
        GetWindowRect(GetActiveWindow(), &rect);
        int centerX = (rect.left + rect.right) / 2;
        int centerY = (rect.top + rect.bottom) / 2;
        
        SetCursorPos(centerX, centerY);
        m_lastMousePos.x = centerX;
        m_lastMousePos.y = centerY;
    } else {
        m_mouseDeltaX = 0.0f;
        m_mouseDeltaY = 0.0f;
    }
}

bool InputManager::IsKeyPressed(int key) const {
    auto current = m_currentKeys.find(key);
    auto previous = m_previousKeys.find(key);
    
    bool currentPressed = (current != m_currentKeys.end()) && current->second;
    bool previousPressed = (previous != m_previousKeys.end()) && previous->second;
    
    return currentPressed && !previousPressed;
}

bool InputManager::IsKeyDown(int key) const {
    auto it = m_currentKeys.find(key);
    return (it != m_currentKeys.end()) && it->second;
}

bool InputManager::IsKeyReleased(int key) const {
    auto current = m_currentKeys.find(key);
    auto previous = m_previousKeys.find(key);
    
    bool currentPressed = (current != m_currentKeys.end()) && current->second;
    bool previousPressed = (previous != m_previousKeys.end()) && previous->second;
    
    return !currentPressed && previousPressed;
}

void InputManager::GetMouseDelta(float& deltaX, float& deltaY) {
    deltaX = m_mouseDeltaX;
    deltaY = m_mouseDeltaY;
}

void InputManager::SetCursorLocked(bool locked) {
    m_cursorLocked = locked;
    if (locked) {
        ShowCursor(FALSE);
        m_firstMouse = true;
    } else {
        ShowCursor(TRUE);
    }
}