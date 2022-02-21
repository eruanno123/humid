// YWROBOT
#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <utils/bit_utils.hpp>
#include <vector>

enum class TextFlow : uint8_t
{
    RightToLeft,
    LeftToRight,
};

enum class LcdBits
{
    NONE = 0x00,
    D0   = 0x01,
    D1   = 0x02,
    D2   = 0x04,
    D3   = 0x08,
    D4   = 0x10,
    D5   = 0x20,
    D6   = 0x40,
    D7   = 0x80,
    RS   = 0x100,
    RW   = 0x200,
    EN   = 0x400,
};

ENABLE_BITMASK_OPERATORS(LcdBits);

class AlphanumericDisplayInterface
{
public:
    virtual ~AlphanumericDisplayInterface() {}
    virtual void setPins(LcdBits pins)     = 0;
    virtual void setBacklight(uint8_t pct) = 0;
};

using AlphanumericPattern = std::vector<uint8_t>;

class AlphanumericDisplay
{
    using Self = AlphanumericDisplay;

public:
    using PinSetter = std::function<void(LcdBits)>;

    AlphanumericDisplay(
        AlphanumericDisplayInterface& lowLevelInterface,
        unsigned numColumns,
        unsigned numRows) :
        m_lowLevel(lowLevelInterface), m_numColumns(numColumns), m_numRows(numRows)
    {
        initialize();
    }

    ~AlphanumericDisplay()
    {
        try
        {
            displayOff();
            backlightOff();
        }
        catch (const std::exception&)
        {
        }
    }

    AlphanumericDisplay(const AlphanumericDisplay&) = delete;
    AlphanumericDisplay& operator=(const AlphanumericDisplay&) = delete;

    //
    // LCD API
    //

    Self& clear();
    Self& home();
    Self& displayOn();
    Self& displayOff();
    Self& blinkOn();
    Self& blinkOff();
    Self& cursorOn();
    Self& cursorOff();
    Self& cursorAt(unsigned row, unsigned column);
    Self& backlightOn();
    Self& backlightOff();
    Self& scrollLeft();
    Self& scrollRight();
    Self& printLeft();
    Self& printRight();
    Self& flow(TextFlow flow);
    Self& shiftIncrement();
    Self& shiftDecrement();
    Self& autoScrollOn();
    Self& autoScrollOff();
    Self& setCustomPattern(unsigned index, const AlphanumericPattern& pattern);
    Self& print(char ch);
    Self& print(const char* str);

private:
    void initialize();
    void sendCommand(LcdBits pins);
    void sendByte(uint8_t value, LcdBits modeBits);
    void sendNibble(LcdBits nibbleBits);
    void setPins(LcdBits pins) { m_lowLevel.setPins(pins); }
    void setBacklight(uint8_t pct) { m_lowLevel.setBacklight(pct); }

    AlphanumericDisplayInterface& m_lowLevel;
    uint8_t m_numColumns;
    uint8_t m_numRows;
    LcdBits m_displayControl;
    LcdBits m_displayMode;
};

#endif
