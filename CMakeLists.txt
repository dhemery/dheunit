cmake_minimum_required(VERSION 3.15)
project(dheunit)

set(CMAKE_CXX_STANDARD 11)

add_library(dheunit)

target_sources(dheunit
        INTERFACE
        context.h
        dheunit.h
        suite.h
        test.h
        PRIVATE
        context.cpp
        dheunit.cpp
        suite.cpp
        test.cpp
        )

add_executable(example)

target_sources(example
        PRIVATE
        example.cpp
        )
target_link_libraries(example
        PRIVATE
        dheunit
        )
