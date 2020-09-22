#include "TeknikioDevice.h"

TeknikioDevice device;

int main()
{
    device.init();

    while(1)
    {
        nano.io.led.setDigitalValue(1);
        nano.sleep(200);

        nano.io.led.setDigitalValue(0);
        nano.sleep(200);
    }
}