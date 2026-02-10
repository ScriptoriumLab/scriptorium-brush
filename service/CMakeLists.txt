add_library(modian_brush_service STATIC
    src/input_protocol_service.cpp
)

target_link_libraries(modian_brush_service PRIVATE
    modian::brush::core
)

target_include_directories(modian_brush_service PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(modian::brush::service ALIAS modian_brush_service)