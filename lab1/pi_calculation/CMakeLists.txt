cmake_minimum_required(VERSION 3.28)
project(pi_calc LANGUAGES CXX)

# Find MPI (works for OpenMPI, MPICH, etc.)
find_package(MPI REQUIRED)

add_executable(${PROJECT_NAME} pi_calc.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE MPI::MPI_CXX debug_utils)

message(STATUS "MPI CXX compiler: ${MPI_CXX_COMPILER}")
message(STATUS "MPI include path: ${MPI_CXX_INCLUDE_PATH}")