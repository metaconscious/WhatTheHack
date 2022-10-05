cmake_minimum_required(VERSION 3.23)
project(WhatTheHack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS off)

add_subdirectory(utility)
add_subdirectory(game)
add_subdirectory(app)
add_subdirectory(dll)
add_subdirectory(injector)
