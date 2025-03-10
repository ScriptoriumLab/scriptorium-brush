add_library(modian_ui STATIC
    src/win32/renderer/candidate_renderer.cpp
    src/win32/candidate_window.cpp
    src/win32/graphic_context.cpp
)

target_include_directories(modian_ui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(modian_ui PRIVATE
    modian_core
)
