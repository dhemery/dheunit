cmake_minimum_required(VERSION 3.15)
project(dheunit)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_subdirectory(dheunit)
add_subdirectory(test)
add_subdirectory(external)
