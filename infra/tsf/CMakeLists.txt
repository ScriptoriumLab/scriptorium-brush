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
    src/class_factory.cpp
    src/dll_main.cpp
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
    set(MODIAN_DLL "${CMAKE_BINARY_DIR}/infra/tsf/Release/ModianTSF.dll")
    add_custom_target(register_modian
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '\"${MODIAN_DLL}\"' -Verb RunAs"
        COMMENT "Registering Modian DLL"
    )

    add_custom_target(unregister_modian
        COMMAND powershell -Command "Start-Process regsvr32 -ArgumentList '/u','\"${MODIAN_DLL}\"' -Verb RunAs"
        COMMENT "Unregistering Modian DLL"
    )
endif()