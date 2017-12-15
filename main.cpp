#include <iostream>

#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdexcept>

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
        file = open("/dev/i2c-2", O_RDWR);

        if (file < 0)
            throw exception("Unable to open device");

        const auto res = ioctl(file, I2C_SLAVE, dev.dev);
        if (res < 0)
            throw exception("Failed to acquire bus access and/or talk to slave");
    }

    ~I2CDev() {
        close(file);
    }

    void write(const char data) {
        ::write(file, &data, sizeof (char));
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

#define En  0b00000100
#define Rw  0b00000010
#define Rs  0b00000001

int main()
{
    I2CDev display({2,0x27});
    display.write(0x03);
    display.write(0x03);
    display.write(0x03);
    display.write(0x02);

    display.write(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE);
    display.write(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    display.write(LCD_CLEARDISPLAY);
    display.write(LCD_ENTRYMODESET | LCD_ENTRYLEFT);

    return 0;
}
