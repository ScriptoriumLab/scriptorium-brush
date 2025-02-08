include(FetchContent)

set(WIL_BUILD_TESTS OFF CACHE BOOL "Disable WIL tests" FORCE)
set(WIL_ENABLE_DOCS OFF CACHE BOOL "Disable WIL docs" FORCE)

FetchContent_Declare(
    WIL
    GIT_REPOSITORY https://github.com/microsoft/wil.git
)
FetchContent_MakeAvailable(WIL)

add_library(modian SHARED
    src/tsf_text_service.cpp
    src/tsf_key_event_service.cpp
    src/class_factory.cpp
    src/dll/main.cpp
)

target_include_directories(modian PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${WIL_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(modian
    PRIVATE
    modian_core
    ole32
    delayimp
    runtimeobject.lib
)

target_compile_definitions(modian PRIVATE _WIN32_DCOM=1)
target_compile_options(modian PRIVATE /W4 /EHsc)
target_link_options(modian PRIVATE "/delayload:msctf.dll")

set_target_properties(modian PROPERTIES
    OUTPUT_NAME "ModianTSF"
)

if(WIN32)
    # 定义 DLL 路径
    set(MODIAN_DLL "${CMAKE_BINARY_DIR}/infra/tsf/Release/ModianTSF.dll")
    # 利用环境变量获取当前用户的主目录
    set(USER_HOME "$ENV{USERPROFILE}")
    # 目标词库目录：例如放在 %USERPROFILE%\Modian\Local 下
    set(MODIAN_ROOT_DIR "${USER_HOME}/Modian/")
    set(DICTIONARY_DIR "${USER_HOME}/Modian/Local")
    set(MODIAN_LOG_DIR "${USER_HOME}/Modian/Log")
    set(LOG_FILE "${MODIAN_LOG_DIR}/log")
    target_compile_definitions(modian PRIVATE DICTIONARY_ROOT="${DICTIONARY_DIR}")
    target_compile_definitions(modian PRIVATE LOG_ROOT="${MODIAN_LOG_DIR}/log")

    add_custom_target(register_modian
        # 创建 Modian 根目录、Log 以及本地字典目录
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DICTIONARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${MODIAN_LOG_DIR}"
        COMMAND ${CMAKE_COMMAND} -E echo "Modian Log" > ${LOG_FILE}
        # 拷贝词库文件到目标目录
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/data/pinyin_dictionary.txt" "${DICTIONARY_DIR}/pinyin_dictionary.txt"
        # 使用 regsvr32 注册 Modian
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '\"${MODIAN_DLL}\"' -Verb RunAs"
        COMMENT "Registering Modian DLL and installing dictionary"
    )

    add_custom_target(unregister_modian
        # 取消注册 DLL
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '/u','\"${MODIAN_DLL}\"' -Verb RunAs"
        # 删除 Modian app 相关所有文件
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${MODIAN_ROOT_DIR}"
        COMMENT "Unregistering Modian DLL and removing installed dictionary folder"
    )
endif()