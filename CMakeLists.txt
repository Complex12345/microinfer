cmake_minimum_required(VERSION 3.15)
project(microinfer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_include_directories(microinfer PUBLIC include)

add_executable(microinfer src/main.cpp)
