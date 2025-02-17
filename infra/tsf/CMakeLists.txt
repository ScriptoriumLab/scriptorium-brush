include(FetchContent)

set(WIL_BUILD_TESTS OFF CACHE BOOL "Disable WIL tests" FORCE)
set(WIL_ENABLE_DOCS OFF CACHE BOOL "Disable WIL docs" FORCE)

FetchContent_Declare(
    WIL
    GIT_REPOSITORY https://github.com/microsoft/wil.git
)
FetchContent_MakeAvailable(WIL)

add_library(modian_infra STATIC
    src/tsf_text_service.cpp
    src/tsf_key_event_service.cpp
    src/class_factory.cpp
    src/dll/register.cpp
)

target_include_directories(modian_infra PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${WIL_SOURCE_DIR}/include
)

target_link_libraries(modian_infra
    PRIVATE
    modian_core
    spdlog::spdlog
)
