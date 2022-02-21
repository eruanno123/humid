
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <thread>
#include <utils/quit_handler.hpp>

#include "lcd/alphanumeric_display.hpp"
#include "lcd/alphanumeric_i2c_driver.hpp"
#include "sensors/pi_dht_read.h"

namespace
{
inline void waitForKey()
{
    do
    {
        std::cout << '\n' << "Press a key to continue...";
    } while (std::cin.get() != '\n');
}

inline void initCustomChars(AlphanumericDisplay& lcd)
{
    lcd //
        .setCustomPattern(
            0,
            {
                // 0
                0b00000,
                0b00000,
                0b01110,
                0b01110,
                0b01110,
                0b01110,
                0b00000,
                0b00000,
                // 1
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                // 2
                0b00001,
                0b00011,
                0b00111,
                0b01111,
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                // 3
                0b10000,
                0b11000,
                0b11100,
                0b11110,
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                // 4
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                0b01111,
                0b00111,
                0b00011,
                0b00001,
                // 5
                0b11111,
                0b11111,
                0b11111,
                0b11111,
                0b11110,
                0b11100,
                0b11000,
                0b10000,
                // 6
                0b11111,
                0b11111,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                // 7
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b11111,
                0b11111,
                0b11111,
            })
        .home();
}

using LargePatternRow = uint16_t;
using LargePattern    = std::array<LargePatternRow, 4>;

const std::map<char, LargePattern> LARGE_PATTERNS = {
    // clang-format off

    { '0', {  0x2113,
              0x1001,
              0x1001,
              0x4115, }},

    { '1', {  0x0003,
              0x0001,
              0x0001,
              0x0005, }},

    { '2', {  0x2113,
              0x0771,
              0x2660,
              0x1115, }},

    { '3', {  0x2113,
              0x0771,
              0x0661,
              0x4115, }},

    { '4', {  0x2003,
              0x1771,
              0x6661,
              0x0005, }},

    { '5', {  0x2113,
              0x1770,
              0x6663,
              0x4115, }},

    { '6', {  0x2113,
              0x1770,
              0x1663,
              0x4115, }},

    { '7', {  0x2113,
              0x0001,
              0x0001,
              0x0005, }},

    { '8', {  0x2113,
              0x1771,
              0x1661,
              0x4115, }},

    { '9', {  0x2113,
              0x4771,
              0x0661,
              0x4115, }},

    { '%', {  0x1025,
              0x0250,
              0x2501,
              0x0000, }},

    // clang-format on
};

inline void printLarge(AlphanumericDisplay& lcd, int startColumn, char ch)
{
    if (startColumn >= 20)
        return;

    auto it = LARGE_PATTERNS.find(ch);
    if (it != LARGE_PATTERNS.end())
    {
        unsigned row = 0;

        for (auto rowPattern : it->second)
        {
            lcd.cursorAt(row, startColumn);
            auto curColumn = startColumn;
            for (int i = 3; i >= 0; --i)
            {
                if (curColumn >= 20)
                    break;
                ++curColumn;

                uint8_t chPattern = (rowPattern >> (4 * i)) & 0x7;
                lcd.print(chPattern > 0 ? chPattern : ' ');
            }

            ++row;
        }
    }
    else
    {
        for (unsigned row = 0; row < 4; row++)
        {
            lcd.cursorAt(row, startColumn);
            lcd.print("    ");
        }
    }
}

inline void printLarge(AlphanumericDisplay& lcd, int startColumn, const char* str)
{
    while (*str != '\0')
    {
        printLarge(lcd, startColumn, *str++);
        startColumn += 5;
    }
}

} // namespace

int main()
{
    using namespace std::chrono_literals;

    try
    {
        AlphanumericDisplayI2C driver("/dev/i2c-1", 0x27);
        AlphanumericDisplay lcd(driver, 20, 4);

        initCustomChars(lcd);

        lcd.clear()
            .cursorAt(0, 0)
            .displayOn()
            .backlightOn() //
            .home();

        // for (char ch = '0'; ch <= '9'; ch++)
        // {
        //     lcd.home();
        //     printLarge(lcd, 0, ch);
        //     waitForKey();
        // }

        lcd.clear();

        utils::SignalQuitHandler sigHandler{};

        while (!sigHandler.quitRequested())
        {
            float humidity1;
            float humidity2;
            float temperature1;
            float temperature2;

            char buf[20];

            int attempts = 10;
            while (attempts-- > 0)
            {
                if (DHT_SUCCESS == pi_dht_read(DHT11, 11, &humidity1, &temperature1))
                    break;
            }

            attempts = 10;
            while (attempts-- > 0)
            {
                if (DHT_SUCCESS == pi_dht_read(DHT11, 17, &humidity2, &temperature2))
                    break;
            }

            using std::chrono::system_clock;

#if 0
            snprintf(buf, sizeof(buf), "Sensor #1");
            lcd.cursorAt(0, 0);
            lcd.print(buf);

            snprintf(buf, sizeof(buf), "RH%%: %3.0f T: %3.0f", humidity1, temperature1);
            lcd.cursorAt(1, 0);
            lcd.print(buf);

            snprintf(buf, sizeof(buf), "Sensor #2");
            lcd.cursorAt(2, 0);
            lcd.print(buf);

            snprintf(buf, sizeof(buf), "RH%%: %3.0f T: %3.0f", humidity2, temperature2);

            lcd.cursorAt(3, 0);
            lcd.print(buf);
#endif

            snprintf(buf, sizeof(buf), "%d%%", int(humidity1));
            lcd.home();
            printLarge(lcd, 0, buf);
            snprintf(buf, sizeof(buf), "%d C", int(temperature1));
            lcd.cursorAt(0, 16).print(buf);

            std::this_thread::sleep_for(1s);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal: " << e.what() << std::endl;
    }
}
