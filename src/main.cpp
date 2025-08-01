#include "core/Application.h"
#include <iostream>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Application app;

    if (!app.Initialize()) {
        MessageBoxA(nullptr, "Failed to initialize application", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    app.Run();
    app.Shutdown();

    return 0;
}