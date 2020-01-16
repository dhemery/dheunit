add_library(suite OBJECT)

target_include_directories(suite
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_sources(suite
        PUBLIC
        context.h
        suite.h
        test.h

        PRIVATE
        context.cpp
        suite.cpp
        test.cpp
        )

target_link_libraries(suite
        PUBLIC
        snowhouse
        )
