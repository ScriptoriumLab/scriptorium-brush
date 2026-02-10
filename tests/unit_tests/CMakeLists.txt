add_executable(modian_brush_unit_tests
    core/protocol/instruction_test.cpp
    service/input_protocol_service_test.cpp
    infra/tsf/dll_main_test.cpp
    infra/ipc/input_protocol_pipe_client_test.cpp
)

target_include_directories(modian_brush_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/tests/unit_tests/include)

target_link_libraries(modian_brush_unit_tests PRIVATE
    modian::brush::core
    modian::brush::service
    modian::brush::ipc
    modian::brush::tsf
    modian::brush
    gtest_main
    gmock_main
    Advapi32
)

target_compile_definitions(modian_brush_unit_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_custom_command(TARGET modian_brush_unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/ModianIME/${CMAKE_CFG_INTDIR}/modian-brush.dll" $<TARGET_FILE_DIR:modian_brush_unit_tests>
)