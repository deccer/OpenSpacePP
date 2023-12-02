cmake_minimum_required(VERSION 3.14)

project(OpenSpacePP)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)
add_subdirectory(src/Engine)
add_subdirectory(src/GameClient)

enable_testing()
