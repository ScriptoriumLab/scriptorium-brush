add_library(modian_manager STATIC
    src/engine_manager.cpp
)

target_link_libraries(
    modian_manager
    PRIVATE
    modian_core
)

target_include_directories(modian_manager PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
