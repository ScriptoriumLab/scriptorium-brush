add_library(modian_brush SHARED
    dll_main.cpp
    server.cpp
)

target_include_directories(modian_brush PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(modian_brush
    PRIVATE
    modian::brush::tsf
    modian::brush::logger
    modian::brush::core
    ole32
    delayimp
    runtimeobject.lib
)

target_compile_definitions(modian_brush PRIVATE _WIN32_DCOM=1)
target_compile_options(modian_brush PRIVATE /W4 /EHsc)
target_link_options(modian_brush PRIVATE "/delayload:msctf.dll")

set_target_properties(modian_brush PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ModianIME"
    OUTPUT_NAME "modian-brush"
)

add_library(modian::brush ALIAS modian_brush)

if(WIN32)
    set(MODIAN_BRUSH_DLL $<TARGET_FILE:modian_brush>)

    set(USER_HOME "$ENV{USERPROFILE}")
    # 将路径分隔符统一为 CMake 友好的格式，或者使用 file(TO_NATIVE_PATH ...)
    set(MODIAN_LOG_DIR "${USER_HOME}/Modian/Log")
    # Log 文件让 spdlog 自己创建即可，这里定义变量备用
    set(MODIAN_BRUSH_LOG_FILE "${MODIAN_LOG_DIR}/modian-brush.log")

    add_custom_target(register_modian_brush
        # 1. 确保目录存在
        COMMAND ${CMAKE_COMMAND} -E make_directory "${MODIAN_LOG_DIR}"

        # 3. 注册 DLL
        # /s: Silent (不弹窗)
        # -Wait: 等待进程结束
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '/s', '\"${MODIAN_BRUSH_DLL}\"' -Verb RunAs -Wait"

        COMMENT "Registering Modian Brush DLL and installing dictionary..."
    )

    add_custom_target(unregister_modian_brush
        COMMAND powershell -Command "tasklist /m modian-brush.dll"

        COMMAND powershell -Command "Stop-Process -Name ctfmon -Force -ErrorAction SilentlyContinue; exit 0"
        COMMAND powershell -Command "Stop-Process -Name regsvr32 -Force -ErrorAction SilentlyContinue; exit 0"
        COMMAND powershell -Command "Stop-Process -Name TextInputHost -Force -ErrorAction SilentlyContinue; exit 0"

        # 反注册 DLL
        # /u: Unregister
        # /s: Silent
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '/u', '/s', '\"${MODIAN_BRUSH_DLL}\"' -Verb RunAs -Wait"

        COMMAND ${CMAKE_COMMAND} -E remove "${MODIAN_BRUSH_LOG_FILE}"

        COMMENT "Unregistering Modian Brush DLL (Logs and Dictionary are PRESERVED for debugging)"
    )
endif()