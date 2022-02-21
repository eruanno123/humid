# ---------------------------------------------------------------------------------------
# PackageName:                  loguru
# PackageAuthor:                Emil Ernerfeldt
# PackageLicenseDeclared:       Public domain
# PackageLocation:              git+https://github.com/emilk/loguru
# PackageLicenseLocation:
# ---------------------------------------------------------------------------------------

CPMAddPackage ("gh:emilk/loguru@2.1.0")

if (loguru_ADDED AND NOT TARGET loguru)

set (THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package (Threads REQUIRED)

    add_library (
        loguru
        STATIC
            ${loguru_SOURCE_DIR}/loguru.cpp
    )

    target_include_directories (
        loguru
        PUBLIC
            ${loguru_SOURCE_DIR}
    )

    target_link_libraries (
        loguru
        PUBLIC
            Threads::Threads
            dl
            fmt::fmt
    )

    target_compile_definitions (
        loguru
        PUBLIC
            LOGURU_WITH_STREAMS=1
            LOGURU_USE_FMTLIB=1
    )

    target_compile_options (
        loguru
        PRIVATE
            -Wno-format-truncation
    )

endif ()
