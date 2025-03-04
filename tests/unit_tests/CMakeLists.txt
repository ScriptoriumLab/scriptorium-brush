include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(modian_unit_tests
    core/engine/input_engine_test.cpp
    infra/tsf/dll_main_test.cpp
    infra/tsf/tsf_key_event_service_test.cpp
    infra/ui/candidate_window_test.cpp
)

target_include_directories(modian_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/tests/unit_tests/include)

target_link_libraries(modian_unit_tests PRIVATE
    modian_core
    modian_manager
    modian_tsf
    modian_ui
    modian
    gtest_main
    gmock_main
    Advapi32
)

target_compile_definitions(modian_unit_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_custom_command(TARGET modian_unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/ModianIME/${CMAKE_CFG_INTDIR}/ModianTSF.dll" $<TARGET_FILE_DIR:modian_unit_tests>
)