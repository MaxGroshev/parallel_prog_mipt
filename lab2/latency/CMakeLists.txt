cmake_minimum_required(VERSION 3.28)
project(latency2 LANGUAGES CXX)


add_executable(${PROJECT_NAME} latency.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE debug_utils)
