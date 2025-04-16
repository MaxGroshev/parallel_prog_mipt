cmake_minimum_required(VERSION 3.28)
project(Par_prog LANGUAGES CXX)

#-----------------------------------------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TESTING_ENABLED ON CACHE BOOL [FORCE])
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++20)

#-----------------------------------------------------------------------------------------

add_subdirectory(debug_utils)
add_subdirectory(lab1)
add_subdirectory(lab2)


unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)


