# System config
set (CMAKE_SYSTEM_NAME      "Linux")
set (CMAKE_SYSTEM_PROCESSOR armv7)

set (CMAKE_SYSROOT          ${CMAKE_SOURCE_DIR}/_sysroot)

set (toolchain_name arm-linux-gnueabihf)
set (CMAKE_C_COMPILER       ${toolchain_name}-gcc)
set (CMAKE_CXX_COMPILER     ${toolchain_name}-g++)
set (CMAKE_Fortran_COMPILER ${toolchain_name}-gfortran)
set (CMAKE_LINKER           ${toolchain_name}-ld)
set (CMAKE_OBJCOPY          ${toolchain_name}-objcopy)
set (CMAKE_AR               ${toolchain_name}-gcc-ar)
set (CMAKE_SIZE             ${toolchain_name}-size)

set (PACKAGE_ARCHITECTURE armhf)
