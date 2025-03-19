add_executable(modian_imgui_ui_test WIN32
    imgui/ui_test_main.cpp
)

target_link_libraries(modian_imgui_ui_test PRIVATE
    modian_core
    modian_manager
    modian_tsf
    modian_ui
    glfw
)

target_compile_definitions(modian_imgui_ui_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
