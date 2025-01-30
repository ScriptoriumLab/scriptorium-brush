add_library(modian_core STATIC
    src/input_engine.cpp
)

target_include_directories(modian_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)