 
#pragma once
#include <windows.h>
#include <string>

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool Initialize();
    void PollEvents();
    bool ShouldClose() const;
    void SwapBuffers();
    
    HWND GetHandle() const { return m_hwnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd;
    std::string m_title;
    int m_width;
    int m_height;
    bool m_shouldClose;
    static Window* s_instance;
};