add_executable(modian_brush_integration_tests
    test_template.cpp
)

target_link_libraries(modian_brush_integration_tests PRIVATE
        modian_brush_core
        modian_brush_manager
        modian_brush_ipc
        modian_brush_tsf
        modian_brush
        gtest_main
        gmock_main
        Advapi32
)

target_compile_definitions(modian_brush_integration_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_custom_command(TARGET modian_brush_integration_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/ModianIME/${CMAKE_CFG_INTDIR}/modian-brush.dll" $<TARGET_FILE_DIR:modian_brush_unit_tests>
)