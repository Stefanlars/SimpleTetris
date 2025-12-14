cmake_minimum_required(VERSION 4.0)
project(SimpleTetris)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SimpleTetris
        main.cpp
        Blocks/Block.cpp
        Blocks/Block.h
        GameManager/sceneRenderer.cpp
        GameManager/sceneRenderer.h
)
