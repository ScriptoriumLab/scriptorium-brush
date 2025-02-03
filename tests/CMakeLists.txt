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
    infra/tsf/dll_main_test.cpp
)

target_link_libraries(modian_test
    PRIVATE
    modian_core
    modian
    gtest_main
    gmock_main
    Advapi32
)

add_custom_command(TARGET modian_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/infra/tsf/Release/ModianTSF.dll" $<TARGET_FILE_DIR:modian_test>
)

add_test(NAME core_tests COMMAND modian_test)