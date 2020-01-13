add_library(runner OBJECT)

target_sources(runner
        PRIVATE
        main.cpp
        )

target_link_libraries(runner
        PUBLIC
        suite
        PRIVATE
        cxxopts
        )
