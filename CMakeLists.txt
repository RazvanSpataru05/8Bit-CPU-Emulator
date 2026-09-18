cmake_minimum_required(VERSION 3.20)

project(CPU-Simulator-8bit
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# Source files
# ============================================================

set(SOURCES
    src/Main.cpp

    src/Assembler/ISAEntry.cpp
    src/Assembler/Lexer.cpp
    src/Assembler/Parser.cpp

    src/Core/CPU.cpp
    src/Core/MemoryUnit.cpp

    src/Debugger/Dissasembler.cpp

    src/IO/ProgramLoader.cpp

    src/UI/UIEditor.cpp
    src/UI/Application.cpp

    src/Utils/CharacterUtils.cpp
    src/Utils/StringUtils.cpp
)

set(HEADERS
    inc/Assembler/InstructionDef.h
    inc/Assembler/ISAEntry.h
    inc/Assembler/Lexer.h
    inc/Assembler/Parser.h
    inc/Assembler/Token.h
    inc/Assembler/Error.h

    inc/Core/CPU.h
    inc/Core/MemoryUnit.h

    inc/Debugger/Dissasembler.h

    inc/IO/ProgramLoader.h

    inc/UI/UIEditor.h
    inc/UI/Application.h

    inc/Utils/CharacterUtils.h
    inc/Utils/StringUtils.h
)

# ============================================================
# Third-party (ImGui)
# ============================================================

set(IMGUI_SOURCES
    lib/ImGui/imgui.cpp
    lib/ImGui/imgui_demo.cpp
    lib/ImGui/imgui_draw.cpp
    lib/ImGui/imgui_tables.cpp
    lib/ImGui/imgui_widgets.cpp
    lib/ImGui/imgui-SFML.cpp
)

# ============================================================
# Executable
# ============================================================

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${IMGUI_SOURCES}
)

# ============================================================
# Header / include directories
# ============================================================

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/Assembler
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/Debugger
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/UI

        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/ImGui
)

# ============================================================
# SFML
# ============================================================

set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${SFML_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SFML_DIR}/sfml-graphics-d.lib
        ${SFML_DIR}/sfml-window-d.lib
        ${SFML_DIR}/sfml-system-d.lib
        opengl32
)

# ============================================================
# Solution Explorer folder structure
# ============================================================

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})
source_group("ImGui" FILES ${IMGUI_SOURCES})

# ============================================================
# MSVC configuration
# ============================================================

if(MSVC)
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /W3
            /permissive-
        )
endif()