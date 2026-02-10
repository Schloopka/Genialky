cmake_minimum_required(VERSION 3.28)
project(Genialky LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(SOURCES
    bishop.cpp
    buttons.cpp
    game.cpp
    Genialky.cpp
    king.cpp
    knight.cpp
    menu.cpp
    pawn.cpp
    queen.cpp
    rook.cpp
)
# Use OpenCV CMake config (works with vcpkg)
find_package(OpenCV CONFIG REQUIRED)
add_executable(Genialky ${SOURCES})
target_compile_features(Genialky PRIVATE cxx_std_17)
target_include_directories(Genialky PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Genialky PRIVATE SFML::Graphics ${OpenCV_LIBS})

target_include_directories(Genialky PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Genialky)
