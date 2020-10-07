#include "TeknikioDevice.h"

TeknikioDevice device;

int main()
{
    device.init();

    while(1)
    {
        device.io.P29.setDigitalValue(1);
        device.sleep(200);

        // device.io.P29.setDigitalValue(0);
        // device.sleep(200);
    }
}

//python uf2convert.py NRF52840_DK.hex -c -f 0xADA52840 -o bluebird_test.uf2