cmake_minimum_required(VERSION 3.16)
project(FOODIE_PartA_Routing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(routing
    main.cpp
    src/Graph.cpp
    src/Robot.cpp
    src/Planner.cpp
)

target_include_directories(routing PRIVATE include)
