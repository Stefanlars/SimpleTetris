cmake_minimum_required(VERSION 4.0)
project(SimpleTetris)

set(CMAKE_CXX_STANDARD 14)


include_directories(.)


find_package(unofficial-pdcurses CONFIG REQUIRED)

add_executable(SimpleTetris
        main.cpp
        Blocks/Block.cpp
        Blocks/Block.h
        GameManager/sceneRenderer.cpp
        GameManager/sceneRenderer.h
)

target_link_libraries(SimpleTetris PRIVATE unofficial::pdcurses::pdcurses)
