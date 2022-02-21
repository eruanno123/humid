
#include "alphanumeric_i2c_driver.hpp"

#include <cassert>
#include <chrono>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <inttypes.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>

AlphanumericDisplayI2C::AlphanumericDisplayI2C(
    const std::string& dev,
    unsigned devAddress) :
    m_fd(open(dev.c_str(), O_RDWR))
{
    if (m_fd < 0)
    {
        throw std::runtime_error("Cannot open i2c device!");
    }

    if (::ioctl(m_fd, I2C_SLAVE, devAddress) < 0)
    {
        throw std::runtime_error("Cannot set i2c address!");
    }
}

AlphanumericDisplayI2C::~AlphanumericDisplayI2C()
{
    close(m_fd);
}

void AlphanumericDisplayI2C::write(uint8_t data)
{
    uint8_t byte = data;

#if 0
    printf("%02X: ", data);

    for (unsigned i = 0; i < 8; i++)
    {
        if (data & (1 << i))
            printf(" -|");
        else
            printf("|- ");

        printf("  ");
    }
    printf("\n");
#endif

    if (::write(m_fd, &byte, 1) != 1)
    {
        std::runtime_error("Cannot write i2c device!");
    }
}

void AlphanumericDisplayI2C::setPins(LcdBits pins)
{
    uint8_t bits = 0;

    if (bool(pins & LcdBits::D0))
        bits |= 0x10;
    if (bool(pins & LcdBits::D1))
        bits |= 0x20;
    if (bool(pins & LcdBits::D2))
        bits |= 0x40;
    if (bool(pins & LcdBits::D3))
        bits |= 0x80;
    if (bool(pins & LcdBits::RS))
        bits |= 0x01;
    if (bool(pins & LcdBits::RW))
        bits |= 0x02;
    if (bool(pins & LcdBits::EN))
        bits |= 0x04;

    write(bits | m_backLightMask);
}

void AlphanumericDisplayI2C::setBacklight(uint8_t pct)
{
    m_backLightMask = (pct > 0) ? 0x08 : 0x00;
    setPins(LcdBits::NONE);
}
