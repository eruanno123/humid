#ifndef ALPHANUMERIC_I2C_DRIVER_GUARD__
#define ALPHANUMERIC_I2C_DRIVER_GUARD__

#include "alphanumeric_display.hpp"

class AlphanumericDisplayI2C : public AlphanumericDisplayInterface
{
public:
    AlphanumericDisplayI2C(const std::string& dev, unsigned devAddress);
    ~AlphanumericDisplayI2C();

    void setPins(LcdBits pins)     override;
    void setBacklight(uint8_t pct) override;

private:
    int m_fd;
    uint8_t m_backLightMask;

    void write(uint8_t byte);
};

#endif /* ALPHANUMERIC_I2C_DRIVER_GUARD__ */
