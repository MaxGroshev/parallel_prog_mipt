cmake_minimum_required(VERSION 3.28)
project(integrate LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp integrate.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE debug_utils)
