include(FetchContent)

set(WIL_BUILD_TESTS OFF CACHE BOOL "Disable WIL tests" FORCE)
set(WIL_ENABLE_DOCS OFF CACHE BOOL "Disable WIL docs" FORCE)

FetchContent_Declare(
    WIL
    GIT_REPOSITORY https://github.com/microsoft/wil.git
)
FetchContent_MakeAvailable(WIL)

add_library(modian_brush_tsf STATIC
    src/tsf_text_service.cpp
    src/tsf_key_event_service.cpp
    src/tsf_edit_session.cpp
    src/class_factory.cpp
    src/dll/register.cpp
    src/utils/utils.cpp
)

target_include_directories(modian_brush_tsf PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
        ${WIL_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/infra/observer/include
)

target_link_libraries(modian_brush_tsf PRIVATE
    modian::brush::core
    modian_brush_service
    modian_brush_logger
    modian_brush_ipc
)
