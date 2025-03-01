cmake_minimum_required(VERSION 3.30)
project(Modian-win LANGUAGES CXX)

# global config
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
    # force to use UTF-8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

    # add UNICODE definition
    add_definitions(/D_UNICODE=1 /DUNICODE=1)
else()
    # set encode in gcc/clang
    add_compile_options(-finput-charset=UTF-8)
    add_compile_options(-fexec-charset=UTF-8)
    add_compile_options(-fwide-exec-charset=UTF-8)

    # set support in wchar
    if(WIN32)
        add_definitions(-municode)  # set for MinGW
    endif()
endif()

# add subdirectory
add_subdirectory(core)
add_subdirectory(manager)
add_subdirectory(infra)
add_subdirectory(app)
add_subdirectory(tests)