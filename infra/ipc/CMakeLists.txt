add_library(modian_brush_ipc STATIC
    src/input_protocol_pipe_client.cpp
)

target_include_directories(modian_brush_ipc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(modian_brush_ipc
    PRIVATE
    modian::brush::core
)

add_library(modian::brush::ipc ALIAS modian_brush_ipc)