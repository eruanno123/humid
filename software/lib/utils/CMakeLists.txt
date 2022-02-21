
set (library_name utils)

add_library (
    ${library_name}
    STATIC
        src/quit_handler.cpp
)

target_include_directories (
    ${library_name}
    PUBLIC
        include
)
