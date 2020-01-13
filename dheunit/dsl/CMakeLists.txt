add_library(dsl OBJECT)

target_include_directories(dsl
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_sources(dsl
        PUBLIC
        dheunit.h
        PRIVATE
        dheunit.cpp
        )

target_link_libraries(dsl
        PUBLIC
        suite
        )
