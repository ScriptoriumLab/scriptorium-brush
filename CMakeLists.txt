cmake_minimum_required(VERSION 3.30)
project(Modian-win LANGUAGES CXX)

# global config
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# add subdirectory
add_subdirectory(core)
add_subdirectory(infra/tsf)
add_subdirectory(tests)

# find Windows SDK
# find_package(WindowsSDK REQUIRED) # add later