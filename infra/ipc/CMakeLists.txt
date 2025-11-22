add_library(modian_brush_ipc STATIC
    src/ipc_client.cpp
)

target_include_directories(modian_brush_ipc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(modian_brush_ipc
    PRIVATE
    modian_brush_core
)

