add_library(modian_ui STATIC
    src/renderer/win32_candidate_renderer.cpp
    src/candidate_window.cpp
)

target_include_directories(modian_ui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
