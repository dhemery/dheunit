add_executable(suite-tests)

target_link_libraries(suite-tests
        PRIVATE
        dsl
        suite
        runner
        )

target_sources(suite-tests
        PRIVATE
        suite-tests.cpp
        test-tests.cpp
        )

add_test(NAME test-suite COMMAND suite-tests)
