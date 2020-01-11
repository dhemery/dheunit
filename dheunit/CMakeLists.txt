add_library(dheunit)

target_include_directories(dheunit
        PUBLIC
        include
        PRIVATE
        src
        )

target_sources(dheunit
        INTERFACE
        include/dheunit.h
        PRIVATE
        src/context.cpp
        src/context.h
        src/dheunit.cpp
        src/suite.cpp
        src/suite.h
        src/test.cpp
        src/test.h
        )
