include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(modian_test
    core/input_engine_test.cpp
)

target_link_libraries(modian_test
    PRIVATE
    modian_core
    gtest_main
    gmock_main
)

add_test(NAME core_tests COMMAND modian_test)