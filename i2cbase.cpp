#include "i2cbase.h"

#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

I2CDev::I2CDev(const DeviceSpec& dev) {
    file = open((std::string("/dev/i2c-") + std::to_string(dev.bus)).c_str(), O_RDWR);

    if (file < 0)
        throw exception("Unable to open bus");

    const auto res = ioctl(file, I2C_SLAVE, dev.dev);
    if (res < 0)
        throw exception("Failed to acquire bus access and/or talk to slave");
}

I2CDev::~I2CDev() {
    close(file);
}

void I2CDev::write(const char data) {
    ::write(file, &data, sizeof (char));
}
