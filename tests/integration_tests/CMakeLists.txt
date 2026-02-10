set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    modian_inkstone
    GIT_REPOSITORY https://github.com/ModianIME/modian-inkstone
    GIT_TAG master
)

FetchContent_MakeAvailable(modian_inkstone)

set(BUILD_TESTING ON CACHE BOOL "" FORCE)

add_executable(modian_brush_integration_tests
    modian_brush_integration_test.cpp
)

target_link_libraries(modian_brush_integration_tests PRIVATE
    modian::brush::core
    modian::brush::ipc
    modian::brush::tsf
    modian::brush
    gtest_main
    gmock_main
    Advapi32
)

set(RAW_USER_HOME "$ENV{USERPROFILE}")
file(TO_CMAKE_PATH "${RAW_USER_HOME}" USER_HOME)
set(MODIAN_ROOT_DIR "${USER_HOME}/Modian")
target_compile_definitions(modian_brush_integration_tests PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    INKSTONE_EXE_PATH="$<TARGET_FILE:modian_inkstone>"
    MODIAN_TEST_ROOT_DIR="${MODIAN_ROOT_DIR}"
)

add_dependencies(modian_brush_integration_tests
    modian::inkstone
    set_up_modian_inkstone_env
)

add_custom_command(TARGET modian_brush_integration_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/ModianIME/${CMAKE_CFG_INTDIR}/modian-brush.dll" $<TARGET_FILE_DIR:modian_brush_integration_tests>
)