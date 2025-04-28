cmake_minimum_required(VERSION 3.28)
project(sort LANGUAGES CXX)

add_executable(${PROJECT_NAME} sort.cpp)

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    debug_utils
    Boost::boost 
    Boost::system 
)
add_subdirectory(./tests/)

