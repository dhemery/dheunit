include(FetchContent)

FetchContent_Declare(snowhouse
        GIT_REPOSITORY https://github.com/banditcpp/snowhouse
        # tag: v4.0.0
        GIT_TAG cd0761b31a5bb2810a5a250a1951224257f596ce
        )

add_subdirectory(snowhouse)
