# VoidStrike

VoidStrike is a fast-paced, wave-based first-person shooter built from the ground up using C++ and the DirectX 11 API. The project showcases a custom game engine architecture, including a 3D renderer, a simple physics and collision system, and core gameplay mechanics. Dive into the arena, survive against relentless waves of enemies, and put your skills to the test!

---

## Features

- **Custom Game Engine**: Built entirely in C++ with no external engine dependencies.
- **DirectX 11 Renderer**: Features a rendering pipeline with vertex and pixel shaders written in HLSL.
- **First-Person Mechanics**: Includes standard FPS controls with mouse-look, WASD movement, and shooting.
- **Dynamic Wave Spawning**: Face off against an ever-present threat as new enemies spawn into the arena.
- **Simple Enemy AI**: Enemies will track and chase the player, creating a constant challenge.
- **UI System**: An in-game UI displays critical information like the player's health bar and a crosshair.

---

## Controls

| Key | Action |
| :--- | :--- |
| **WASD** | Move Player |
| **Mouse** | Look Around |
| **Left Click** | Shoot |
| **Escape** | Quit Game |

---

## Getting Started

To build and run this project, you will need a Windows environment with Visual Studio and the Windows 10/11 SDK installed.

### Prerequisites

-   Windows 10 or Windows 11
-   [Visual Studio 2019 or later](https://visualstudio.microsoft.com/) (with C++ workload)
-   [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) (usually included with Visual Studio)
-   [CMake](https://cmake.org/download/)

### Build Instructions

1.  **Clone the repository:**
   

2.  **Create a build directory:**
    ```sh
    mkdir build
    cd build
    ```

3.  **Run CMake to generate the Visual Studio solution:**
    ```sh
    cmake ..
    ```

4.  **Build the project:**
    Open the `VoidStrike.sln` file in Visual Studio and build the solution (Build > Build Solution), or run the following command from the `build` directory:
    ```sh
    cmake --build . --config Release
    ```

5.  **Run the game:**
    The executable will be located in the `build/Release` directory.
    ```sh
    ./Release/VoidStrike.exe
    ```

---
