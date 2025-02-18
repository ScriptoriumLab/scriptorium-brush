cmake_minimum_required(VERSION 3.30)
project(Modian-win LANGUAGES CXX)

# global config
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_definitions(
    /D_UNICODE=1
    /DUNICODE=1
)

include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.10.0   # 根据需要选择版本
)
FetchContent_MakeAvailable(spdlog)
add_compile_definitions(SPDLOG_HEADER_ONLY)

# add subdirectory
add_subdirectory(core)
add_subdirectory(infra)
add_subdirectory(app)
add_subdirectory(tests)



# find Windows SDK
# find_package(WindowsSDK REQUIRED) # add later