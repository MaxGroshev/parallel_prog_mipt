cmake_minimum_required(VERSION 3.28)
project(trans_eq LANGUAGES CXX)

# Find MPI (works for OpenMPI, MPICH, etc.)
find_package(MPI REQUIRED)

add_executable(${PROJECT_NAME} trans_eq.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE MPI::MPI_CXX debug_utils)
target_include_directories(${PROJECT_NAME} PRIVATE ./include)

message(STATUS "MPI CXX compiler: ${MPI_CXX_COMPILER}")
message(STATUS "MPI include path: ${MPI_CXX_INCLUDE_PATH}")