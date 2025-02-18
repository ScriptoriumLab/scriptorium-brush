add_library(modian_core STATIC
    src/engine/pinyin_engine.cpp
    src/logger/console_logger.cpp
)

target_include_directories(modian_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(modian_core PRIVATE spdlog::spdlog)