# ---------------------------------------------------------------------------------------
# PackageName:                  fmt
# PackageAuthor:                Contributors
# PackageLicenseDeclared:       MIT
# PackageLocation:              git+https://github.com/fmtlib/fmt
# PackageLicenseLocation:       https://github.com/fmtlib/fmt/blob/master/LICENSE.rst
# ---------------------------------------------------------------------------------------

set (fmt_version    "8.1.1")

CPMAddPackage (
    NAME        fmt
    VERSION     ${fmt_version}
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG     ${fmt_version}
    GIT_SHALLOW
)
