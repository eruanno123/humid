// Based on the work by DFRobot

#include "alphanumeric_display.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

namespace Command
{
constexpr LcdBits ClearScreen        = LcdBits::D0;
constexpr LcdBits ReturnHome         = LcdBits::D1;
constexpr LcdBits EntryModeSet       = LcdBits::D2;
constexpr LcdBits DisplayControl     = LcdBits::D3;
constexpr LcdBits DisplayCursorShift = LcdBits::D4;
constexpr LcdBits FunctionSet        = LcdBits::D5;
constexpr LcdBits SetCGRamAddress    = LcdBits::D6;
constexpr LcdBits SetDDRamAddress    = LcdBits::D7;
}; // namespace Command

namespace EntryModeBits
{
constexpr LcdBits Decrement = LcdBits::NONE;
constexpr LcdBits Increment = LcdBits::D1;
constexpr LcdBits Left      = LcdBits::NONE;
constexpr LcdBits Right     = LcdBits::D0;
} // namespace EntryModeBits

namespace DisplayControlBits
{
constexpr LcdBits DisplayOff = LcdBits::NONE;
constexpr LcdBits DisplayOn  = LcdBits::D2;
constexpr LcdBits CursorOff  = LcdBits::NONE;
constexpr LcdBits CursorOn   = LcdBits::D1;
constexpr LcdBits BlinkOff   = LcdBits::NONE;
constexpr LcdBits BlinkOn    = LcdBits::D0;
} // namespace DisplayControlBits

namespace ShiftControlBits
{
constexpr LcdBits MoveCursor  = LcdBits::NONE;
constexpr LcdBits MoveDisplay = LcdBits::D3;
constexpr LcdBits MoveLeft    = LcdBits::NONE;
constexpr LcdBits MoveRight   = LcdBits::D2;
} // namespace ShiftControlBits

namespace FunctionSetBits
{
constexpr LcdBits BitMode4X = LcdBits::NONE;
constexpr LcdBits BitMode8X = LcdBits::D4;
constexpr LcdBits Line1X    = LcdBits::NONE;
constexpr LcdBits Line2X    = LcdBits::D3;
constexpr LcdBits Dots_5x8  = LcdBits::NONE;
constexpr LcdBits Dots_5x10 = LcdBits::D2;
} // namespace FunctionSetBits

namespace detail
{

template <typename Rep, typename Period>
inline void delay(std::chrono::duration<Rep, Period> duration)
{
    std::this_thread::sleep_for(duration);
}

} // namespace detail

using namespace std::chrono_literals;


void AlphanumericDisplay::initialize()
{

    detail::delay(50ms);

    setBacklight(0);
    detail::delay(1s);

    // Sequence for 4-bit mode
    sendNibble(LcdBits::D0 | LcdBits::D1);
    detail::delay(4500us); // wait min 4.1ms
    sendNibble(LcdBits::D0 | LcdBits::D1);
    detail::delay(4500us); // wait min 4.1ms
    sendNibble(LcdBits::D0 | LcdBits::D1);
    detail::delay(150us);
    sendNibble(LcdBits::D1);

    // FunctionSet

    LcdBits displayFunction =
        FunctionSetBits::BitMode4X | FunctionSetBits::Line1X | FunctionSetBits::Dots_5x8;

    if (m_numRows > 1)
    {
        displayFunction |= FunctionSetBits::Line2X;
    }

    sendCommand(Command::FunctionSet | displayFunction);

    displayOn();

    clear();

    flow(TextFlow::LeftToRight);
    autoScrollOff();

    home();
}

AlphanumericDisplay& AlphanumericDisplay::clear()
{
    sendCommand(Command::ClearScreen);
    detail::delay(2000us);
    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::home()
{
    sendCommand(Command::ReturnHome);
    detail::delay(2000us);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::cursorAt(unsigned row, unsigned column)
{
    static const uint8_t ROW_OFFSETS[] = {
        0x00,
        0x40,
        0x14,
        0x54,
    };

    if ((row >= m_numRows) || (column >= m_numColumns))
    {
        throw std::runtime_error("Invalid column or row: " + std::to_string(row) + " " + std::to_string(column));
    }

    sendCommand(
        Command::SetDDRamAddress | static_cast<LcdBits>(ROW_OFFSETS[row] + column));

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::displayOn()
{
    m_displayControl |= DisplayControlBits::DisplayOn;
    sendCommand(Command::DisplayControl | m_displayControl);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::displayOff()
{
    m_displayControl &= ~DisplayControlBits::DisplayOn;
    sendCommand(Command::DisplayControl | m_displayControl);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::cursorOn()
{
    m_displayControl |= DisplayControlBits::CursorOn;
    sendCommand(Command::DisplayControl | m_displayControl);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::cursorOff()
{
    m_displayControl &= ~DisplayControlBits::CursorOn;
    sendCommand(Command::DisplayControl | m_displayControl);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::blinkOn()
{
    m_displayControl |= DisplayControlBits::BlinkOn;
    sendCommand(Command::DisplayControl | m_displayControl);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::blinkOff()
{
    m_displayControl &= ~DisplayControlBits::BlinkOn;
    sendCommand(Command::DisplayControl | m_displayControl);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::scrollLeft(void)
{
    sendCommand(
        Command::DisplayCursorShift //
        | ShiftControlBits::MoveDisplay | ShiftControlBits::MoveLeft);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::scrollRight(void)
{
    sendCommand(
        Command::DisplayCursorShift //
        | ShiftControlBits::MoveDisplay | ShiftControlBits::MoveRight);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::flow(TextFlow flow)
{
    switch (flow)
    {
        case TextFlow::LeftToRight:
        {
            m_displayMode |= EntryModeBits::Left;
            sendCommand(Command::EntryModeSet | m_displayMode);
        }
        break;

        case TextFlow::RightToLeft:
        {
            m_displayMode &= ~EntryModeBits::Left;
            sendCommand(Command::EntryModeSet | m_displayMode);
        }
        break;

        default:
            assert(!"Unexpected TextFlow value");
    }

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::autoScrollOn()
{
    m_displayMode |= EntryModeBits::Increment;
    sendCommand(Command::EntryModeSet | m_displayMode);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::autoScrollOff()
{
    m_displayMode &= ~EntryModeBits::Increment;
    sendCommand(Command::EntryModeSet | m_displayMode);

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::setCustomPattern(
    unsigned index,
    const AlphanumericPattern& pattern)
{
    index &= 0x7;
    sendCommand(Command::SetCGRamAddress | static_cast<LcdBits>(index));

    for (int i = 0; i < pattern.size(); i++)
    {
        sendByte(pattern[i], LcdBits::RS);
    }

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::print(char ch)
{
    sendByte(ch, LcdBits::RS);
    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::print(const char* str)
{
    while (*str != '\0')
    {
        print(*str++);
    }

    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::backlightOn()
{
    setBacklight(100);
    return *this;
}

AlphanumericDisplay& AlphanumericDisplay::backlightOff()
{
    setBacklight(0);
    return *this;
}

inline void AlphanumericDisplay::sendCommand(LcdBits value)
{
    sendByte(static_cast<uint8_t>(value), LcdBits::NONE);
}

void AlphanumericDisplay::sendByte(uint8_t value, LcdBits modeBits)
{
    sendNibble(static_cast<LcdBits>((value >> 4) & 0x0F) | modeBits);
    sendNibble(static_cast<LcdBits>(value & 0xF) | modeBits);
}

void AlphanumericDisplay::sendNibble(LcdBits bits)
{
    setPins(bits | LcdBits::EN);
    detail::delay(1us);
    setPins(bits & ~LcdBits::EN);
    detail::delay(50us);
}
