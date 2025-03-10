add_executable(modian_ui_test WIN32
    ui_test_main.cpp
)

target_link_libraries(modian_ui_test PRIVATE
    modian_ui
    modian_core
)

