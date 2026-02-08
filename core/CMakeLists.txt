add_library(modian_brush_core STATIC
    src/logger/console_logger.cpp
    src/logger/logger_service.cpp
    src/protocol/instruction.cpp
)

target_include_directories(modian_brush_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)