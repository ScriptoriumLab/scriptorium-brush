add_executable(modian_win32_ui_test WIN32
    win32/ui_test_main.cpp
)

target_link_libraries(modian_win32_ui_test PRIVATE
    modian_ui
    modian_core
)

add_executable(modian_imgui_ui_test WIN32
    imgui/ui_test_main.cpp
)

target_link_libraries(modian_imgui_ui_test PRIVATE
    modian_ui
    modian_core
    glfw
)
