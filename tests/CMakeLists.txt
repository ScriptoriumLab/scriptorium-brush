include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(modian_test
    core/engine/input_engine_test.cpp
    infra/tsf/dll_main_test.cpp
    infra/tsf/tsf_key_event_service_test.cpp
)

target_include_directories(modian_test PRIVATE ${CMAKE_SOURCE_DIR}/tests/include)

target_link_libraries(modian_test
    PRIVATE
    modian
    modian_tsf
    modian_core
    gtest_main
    gmock_main
    Advapi32
)

target_compile_definitions(modian_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_custom_command(TARGET modian_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/ModianIME/${CMAKE_CFG_INTDIR}/ModianTSF.dll" $<TARGET_FILE_DIR:modian_test>
)