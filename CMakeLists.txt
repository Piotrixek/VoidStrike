 
cmake_minimum_required(VERSION 3.20)
project(VoidStrike)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_SYSTEM_VERSION 10.0)
endif()

find_package(DirectX QUIET)

set(SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/core/Window.cpp
    src/graphics/Renderer.cpp
    src/graphics/Shader.cpp
    src/graphics/Camera.cpp
    src/graphics/Mesh.cpp
    src/game/Game.cpp
    src/game/Player.cpp
    src/game/Enemy.cpp
    src/game/Map.cpp
    src/game/UI.cpp
    src/input/InputManager.cpp
)

set(HEADERS
    src/core/Application.h
    src/core/Window.h
    src/graphics/Renderer.h
    src/graphics/Shader.h
    src/graphics/Camera.h
    src/graphics/Mesh.h
    src/game/Game.h
    src/game/Player.h
    src/game/Enemy.h
    src/game/Map.h
    src/game/UI.h
    src/math/Vector3.h
    src/math/Matrix4.h
    src/input/InputManager.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE src)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib
        user32.lib
        kernel32.lib
        gdi32.lib
        winspool.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        comdlg32.lib
        advapi32.lib
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

file(GLOB SHADER_FILES "shaders/*.hlsl")
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME} COPYONLY)
endforeach()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()