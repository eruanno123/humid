# ---------------------------------------------------------------------------------------
# PackageName:                  nlohmann_json
# PackageAuthor:                Niels Lohmann
# PackageLicenseDeclared:       MIT
# PackageLocation:              git+https://github.com/nlohmann/json
# PackageLicenseLocation:       https://github.com/nlohmann/json/blob/develop/LICENSE.MIT
# ---------------------------------------------------------------------------------------

CPMAddPackage ("gh:nlohmann/json@3.10.5")

add_library (nlohmann_json_wrapper INTERFACE)

target_include_directories (
    nlohmann_json_wrapper
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/addon
)

target_link_libraries (
    nlohmann_json_wrapper
    INTERFACE
        nlohmann_json::nlohmann_json
)
