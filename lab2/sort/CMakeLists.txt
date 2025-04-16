cmake_minimum_required(VERSION 3.28)
project(sort LANGUAGES CXX)

add_executable(${PROJECT_NAME} sort.cpp)

add_subdirectory(./tests/)

target_link_libraries(${PROJECT_NAME} PRIVATE debug_utils)
