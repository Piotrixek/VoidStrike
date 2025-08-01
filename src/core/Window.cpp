#include "Window.h"
#include <iostream>

Window* Window::s_instance = nullptr;

Window::Window(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height), m_hwnd(nullptr), m_shouldClose(false) {
    s_instance = this;
}

Window::~Window() {
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
    }
}

bool Window::Initialize() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "VoidStrikeWindow";

    if (!RegisterClassEx(&wc)) {
        return false;
    }

    RECT rect = { 0, 0, m_width, m_height };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    std::string wTitle = m_title;

    m_hwnd = CreateWindowExA(
        WS_EX_APPWINDOW,
        "VoidStrikeWindow",
        wTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!m_hwnd) {
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
    SetCursor(LoadCursor(nullptr, IDC_CROSS));

    return true;
}

void Window::PollEvents() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool Window::ShouldClose() const {
    return m_shouldClose;
}

void Window::SwapBuffers() {
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        if (s_instance) {
            s_instance->m_shouldClose = true;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if (s_instance && wParam != SIZE_MINIMIZED) {
            s_instance->m_width = LOWORD(lParam);
            s_instance->m_height = HIWORD(lParam);
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}