add_library(scriptorium_brush SHARED
    dll_main.cpp
    server.cpp
)

target_include_directories(scriptorium_brush PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(scriptorium_brush
    PRIVATE
    scriptorium::felt::core::logger
    scriptorium::felt::infra::logger
    scriptorium::brush::tsf
    ole32
    delayimp
    runtimeobject.lib
)

target_compile_definitions(scriptorium_brush PRIVATE _WIN32_DCOM=1)
target_compile_options(scriptorium_brush PRIVATE /W4 /EHsc)
target_link_options(scriptorium_brush PRIVATE "/delayload:msctf.dll")

set_target_properties(scriptorium_brush PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ScriptoriumLabIME"
    OUTPUT_NAME "scriptorium-brush"
)

add_library(scriptorium::brush ALIAS scriptorium_brush)

if(WIN32)
    set(SCRIPTORIUM_BRUSH_DLL $<TARGET_FILE:scriptorium_brush>)

    set(USER_HOME "$ENV{USERPROFILE}")
    # 将路径分隔符统一为 CMake 友好的格式，或者使用 file(TO_NATIVE_PATH ...)
    set(SCRIPTORIUM_LOG_DIR "${USER_HOME}/Scriptorium/Log")
    # Log 文件让 spdlog 自己创建即可，这里定义变量备用
    set(SCRIPTORIUM_BRUSH_LOG_FILE "${SCRIPTORIUM_LOG_DIR}/scriptorium-brush.log")

    add_custom_target(register_scriptorium_brush
        # 1. 确保目录存在
        COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPTORIUM_LOG_DIR}"

        # 3. 注册 DLL
        # /s: Silent (不弹窗)
        # -Wait: 等待进程结束
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '/s', '\"${SCRIPTORIUM_BRUSH_DLL}\"' -Verb RunAs -Wait"

        COMMENT "Registering Scriptorium Brush DLL and installing dictionary..."
    )

    add_custom_target(unregister_scriptorium_brush
        COMMAND powershell -Command "tasklist /m scriptorium-brush.dll"

        COMMAND powershell -Command "Stop-Process -Name ctfmon -Force -ErrorAction SilentlyContinue; exit 0"
        COMMAND powershell -Command "Stop-Process -Name regsvr32 -Force -ErrorAction SilentlyContinue; exit 0"
        COMMAND powershell -Command "Stop-Process -Name TextInputHost -Force -ErrorAction SilentlyContinue; exit 0"

        # 反注册 DLL
        # /u: Unregister
        # /s: Silent
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '/u', '/s', '\"${SCRIPTORIUM_BRUSH_DLL}\"' -Verb RunAs -Wait"

        COMMAND ${CMAKE_COMMAND} -E remove "${SCRIPTORIUM_BRUSH_LOG_FILE}"

        COMMENT "Unregistering Scriptorium Brush DLL (Logs and Dictionary are PRESERVED for debugging)"
    )
endif()
