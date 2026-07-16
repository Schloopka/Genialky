cmake_minimum_required(VERSION 3.28)
project(Genialky LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_path(SFML_INCLUDE_DIR NAMES SFML/Graphics.hpp REQUIRED)
find_library(SFML_GRAPHICS_LIBRARY NAMES sfml-graphics REQUIRED)
find_library(SFML_WINDOW_LIBRARY NAMES sfml-window REQUIRED)
find_library(SFML_SYSTEM_LIBRARY NAMES sfml-system REQUIRED)

set(SOURCES
    game/Genialky.cpp
    game/game.cpp
    game/Input/buttons.cpp
    game/Input/menu.cpp
    game/Pieces/bishop.cpp
    game/Pieces/king.cpp
    game/Pieces/knight.cpp
    game/Pieces/pawn.cpp
    game/Pieces/piece.cpp
    game/Pieces/queen.cpp
    game/Pieces/rook.cpp
)
add_executable(Genialky ${SOURCES})
target_compile_features(Genialky PRIVATE cxx_std_17)
target_link_libraries(Genialky PRIVATE
    ${SFML_GRAPHICS_LIBRARY}
    ${SFML_WINDOW_LIBRARY}
    ${SFML_SYSTEM_LIBRARY}
)

target_include_directories(Genialky PRIVATE
    ${SFML_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/game
    ${CMAKE_CURRENT_SOURCE_DIR}/game/Input
    ${CMAKE_CURRENT_SOURCE_DIR}/game/Pieces
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Genialky)
