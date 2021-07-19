#include "TeknikioDevice.h"

TeknikioDevice bluebird;

int main()
{
    bluebird.init();

    while(1)
    {
        bluebird.io.P29.setDigitalValue(1);
        bluebird.sleep(200);

        // device.io.P29.setDigitalValue(0);
        // device.sleep(200);
    }
}

// //python uf2convert.py NRF52840_DK.hex -c -f 0xADA52840 -o bluebird_test.uf2