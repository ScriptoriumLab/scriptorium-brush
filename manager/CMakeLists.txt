add_library(modian_brush_manager STATIC
    src/engine_manager.cpp
    src/candidate_manager.cpp
)

target_link_libraries(
    modian_brush_manager
    PRIVATE
    modian_brush_core
)

target_include_directories(modian_brush_manager PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
