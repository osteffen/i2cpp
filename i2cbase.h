#pragma once
#include <stdexcept>

struct DeviceSpec {
    int bus;
    int dev;
};

class I2CDev {
public:

    struct exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    I2CDev(const DeviceSpec& dev);
    ~I2CDev();

    void write(const char data);

protected:
    int file = -1;
};
