add_subdirectory(suite)
add_subdirectory(runner)
add_subdirectory(dsl)

add_library(dheunit STATIC)

target_link_libraries(dheunit
        PUBLIC
        dsl
        PRIVATE
        suite
        runner
        )
