
add_executable (
    humid
        main.cpp
        modules/lcd/alphanumeric_display.cpp
        modules/lcd/alphanumeric_i2c_driver.cpp
        modules/sensors/common_dht_read.c
        modules/sensors/pi_dht_read.c
        modules/sensors/pi_mmio.c
)

target_include_directories (
    humid
    PRIVATE
        modules
)

target_link_libraries (
    humid
    PRIVATE
        utils
)

install (
    TARGETS humid
)
