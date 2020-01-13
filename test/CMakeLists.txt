add_executable(suitetests)

target_link_libraries(suitetests
        PRIVATE
        dsl
        suite
        runner
        )

target_sources(suitetests
        PRIVATE
        suite-test.cpp
        test-test.cpp
        )

add_test(NAME runsuitetests COMMAND suitetests)
