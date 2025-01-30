include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    modian_tests
    test_main.cpp
    test_template.cpp
)

target_link_libraries(
    modian_tests
    modian_core
    gtest_main
    gmock_main
)
