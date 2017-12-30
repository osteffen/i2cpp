#include <iostream>

#include <string>


#include <chrono>
#include <thread>

#include "i2cbase.h"
#include "devices/hd44780.h"

void sleep_ms(const int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main()
{
    Display display({1,0x27});
    display.setBacklight(true);
    sleep_ms(20);

    std::string line;
    while(!std::cin.eof()) {
        std::cin >> line;
        const auto newline = line.find("\n");
        display.display_string(line.substr(0, newline), 1);
        display.display_string(line.substr(newline+1, line.npos));

    }
    display.display_string("hi!sdaf sadf", 1);
    display.display_string("-------dddd--", 2);
    sleep_ms(2000);
    display.setBacklight(false);
    return 0;
}
