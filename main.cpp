#include <iostream>

#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdexcept>
#include <string>


#include <chrono>
#include <thread>


void sleep_ms(const int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

class I2CDev {
public:
    struct DeviceSpec {
        int bus;
        int dev;
    };

    struct exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    I2CDev(const DeviceSpec& dev) {
        file = open("/dev/i2c-1", O_RDWR);

        if (file < 0)
            throw exception("Unable to open bus");

        const auto res = ioctl(file, I2C_SLAVE, dev.dev);
        if (res < 0)
            throw exception("Failed to acquire bus access and/or talk to slave");
    }

    ~I2CDev() {
        close(file);
    }

    void write(const char data) {
        ::write(file, &data, sizeof (char));
        sleep_ms(1);
    }

protected:
    int file = -1;
};

#define LCD_CLEARDISPLAY  0x01
#define LCD_RETURNHOME  0x02
#define LCD_ENTRYMODESET  0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT  0x10
#define LCD_FUNCTIONSET  0x20
#define LCD_SETCGRAMADDR  0x40
#define LCD_SETDDRAMADDR  0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT  0x00
#define LCD_ENTRYLEFT  0x02
#define LCD_ENTRYSHIFTINCREMENT  0x01
#define LCD_ENTRYSHIFTDECREMENT  0x00

// flags for display on/off control
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF  0x00
#define LCD_CURSORON  0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON  0x01
#define LCD_BLINKOFF  0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE  0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT  0x04
#define LCD_MOVELEFT  0x00

// flags for function set
#define LCD_8BITMODE  0x10
#define LCD_4BITMODE  0x00
#define LCD_2LINE  0x08
#define LCD_1LINE  0x00
#define LCD_5x10DOTS  0x04
#define LCD_5x8DOTS  0x00

// flags for backlight control
#define LCD_BACKLIGHT  0x08
#define LCD_NOBACKLIGHT  0x00

#define En  4
#define Rw  2
#define Rs  1

class Display : protected I2CDev {
    public:

    Display(const I2CDev::DeviceSpec& s):I2CDev(s) {
        Init();
    }

    void write(const char data, const char mode=0) {
        write4bit(mode | (data & 0xF0));
        write4bit(mode | ((data << 4) & 0xF0));
    }

    void write4bit(const char data) {
        I2CDev::write(data | LCD_BACKLIGHT);
        strobe(data);
    }

    void strobe(const char data) {
        I2CDev::write(data | En | LCD_BACKLIGHT);
        sleep_ms(5);
        I2CDev::write(((data & ~En) | LCD_BACKLIGHT));
        sleep_ms(1);
    }

    void Init() {
        write(0x03);
        write(0x03);
        write(0x03);
        write(0x02);

        write(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE);
        write(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
        write(LCD_CLEARDISPLAY);
        write(LCD_ENTRYMODESET | LCD_ENTRYLEFT);
    }

    void setBacklight(const bool state) {
        I2CDev::write(state ? LCD_BACKLIGHT : LCD_NOBACKLIGHT);
        sleep_ms(10);
    }

    void clear() {
        write(LCD_CLEARDISPLAY);
        write(LCD_RETURNHOME);
    }

    void display_string(const std::string& text, const int line) {
        if(line == 1)
           write(0x80);
        if(line == 2)
           write(0xC0);
        if(line == 3)
           write(0x94);
        if(line == 4)
           write(0xD4);

        for(const char& c : text) {
            std::cout << c;
           write(c, Rs);
           std::cout << std::endl;
        }

    }
};

int main()
{
    Display display({1,0x27});
    display.setBacklight(true);
    display.display_string("hi!", 1);
    sleep_ms(1000);
    display.setBacklight(false);
    return 0;
}
