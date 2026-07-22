add_executable(scriptorium_brush_unit_tests
    infra/tsf/dll_main_test.cpp
)

target_include_directories(scriptorium_brush_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/tests/unit_tests/include)

target_link_libraries(scriptorium_brush_unit_tests PRIVATE
    scriptorium::felt::core::protocol
    scriptorium::felt::core::ipc
    scriptorium::felt::core::logger
    scriptorium::brush::tsf
    scriptorium::brush
    gtest_main
    gmock_main
    Advapi32
)

target_compile_definitions(scriptorium_brush_unit_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_custom_command(TARGET scriptorium_brush_unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/ScriptoriumLabIME/${CMAKE_CFG_INTDIR}/scriptorium-brush.dll" $<TARGET_FILE_DIR:scriptorium_brush_unit_tests>
)
