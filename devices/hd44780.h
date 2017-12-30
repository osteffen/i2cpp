#pragma once
#include "../i2cbase.h"
#include <string>

class Display {
public:

    Display(const DeviceSpec& s);

    void write(const char data, const char mode=0);
    void setBacklight(const bool state);
    void clear();
    void display_string(const std::string& text, const int line);

private:
    void write4bit(const char data);
    void strobe(const char data);
    void Init();

    I2CDev dev;
};
