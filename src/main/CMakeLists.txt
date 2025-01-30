add_library(
    modian
    SHARED
    main.cpp
)
target_link_libraries(modian PRIVATE modian_core ole32 msctf)
