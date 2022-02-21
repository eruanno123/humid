# ---------------------------------------------------------------------------------------
# PackageName:                  clipp
# PackageAuthor:                André Müller
# PackageLicenseDeclared:       MIT
# PackageLocation:              git+https://github.com/muellan
# PackageLicenseLocation:       https://github.com/nlohmann/json/blob/develop/LICENSE.MIT
# ---------------------------------------------------------------------------------------

CPMAddPackage ("gh:muellan/clipp@1.2.3")

if (clipp_ADDED AND NOT TARGET clipp)
    add_library (clipp INTERFACE)
    target_include_directories (clipp SYSTEM INTERFACE ${clipp_SOURCE_DIR}/include)
endif ()
