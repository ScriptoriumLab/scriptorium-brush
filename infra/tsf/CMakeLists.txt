add_library(modian SHARED
    src/tsf_text_service.cpp
)

target_include_directories(modian PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(modian
    PRIVATE
    modian_core
    ole32
    msctf
    runtimeobject.lib
)

target_compile_definitions(modian PRIVATE _WIN32_DCOM=1)
target_compile_options(modian PRIVATE /W4 /EHsc)