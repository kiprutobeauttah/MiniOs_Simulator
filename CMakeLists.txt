cmake_minimum_required(VERSION 3.5)
project(MiniOS-GUI-Simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(include)

add_executable(MiniOS-GUI-Simulator
    src/main.cpp
    src/process.cpp
    src/scheduler.cpp
    src/memory.cpp
    src/gui.cpp
)

target_link_libraries(MiniOS-GUI-Simulator sfml-graphics sfml-window sfml-system)
