add_library(dheunit)

target_include_directories(dheunit
        PUBLIC
        include
        PRIVATE
        suite
        )

target_sources(dheunit
        INTERFACE
        include/dheunit.h
        PRIVATE
        dsl/dheunit.cpp
        runner/main.cpp
        suite/context.cpp
        suite/context.h
        suite/suite.cpp
        suite/suite.h
        suite/test.cpp
        suite/test.h
        )

target_link_libraries(dheunit
        PUBLIC
        snowhouse
        )
