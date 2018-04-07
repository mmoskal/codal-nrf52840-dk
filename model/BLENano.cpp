/*
The MIT License (MIT)

Copyright (c) 2016 Lancaster University, UK.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#include "BLENano.h"
#include "Timer.h"

using namespace codal;

BLENano *ble_nano_device_instance = NULL;

void early_init(){
    // Ensure NFC pins are configured as GPIO. If not, update the non-volatile UICR.
    if (NRF_UICR->NFCPINS)
    {
        DMESG("RESET UICR\n");
        // Enable Flash Writes
        NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

        // Configure PINS for GPIO use.
        NRF_UICR->NFCPINS = 0;

        // Disable Flash Writes
        NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

        // Reset, so the changes can take effect.
        NVIC_SystemReset();
    }


/*
    uint32_t *old_vectors = (uint32_t *)SCB->VTOR;
    if ((uint32_t)old_vectors < 0x20000000) {
        uint32_t *vectors = (uint32_t*)malloc(NVIC_NUM_VECTORS * 4);
        DMESG("vtor %p -> %p", SCB->VTOR, vectors);
        for (int i = 0; i < NVIC_NUM_VECTORS; i++) {
            vectors[i] = old_vectors[i];
        }
        SCB->VTOR = (uint32_t)vectors;
    }
    */
}

/**
  * Constructor.
  *
  * Create a representation of a BLENano device, which includes member variables
  * that represent various device drivers used to control aspects of the BLENano.
  */
BLENano::BLENano() :
    serial(P0_2, NC),
    messageBus(),
    timer(),
    io(),
    screen(io.spi, io.P28, io.P29)
{
    // Clear our status
    status = 0;
    ble_nano_device_instance = this;

    early_init();

    // Configure serial port for debugging
    //serial.set_flow_control(mbed::Serial::Disabled);
    serial.baud(115200);
}

uint16_t colors[60*70];

/**
  * Post constructor initialisation method.
  *
  * This call will initialised the scheduler, memory allocator and Bluetooth stack.
  *
  * This is required as the Bluetooth stack can't be brought up in a
  * static context i.e. in a constructor.
  *
  * @code
  * uBit.init();
  * @endcode
  *
  * @note This method must be called before user code utilises any functionality
  *       contained within the GenuinoZero class.
  */
int BLENano::init()
{
    if (status & DEVICE_INITIALIZED)
        return DEVICE_NOT_SUPPORTED;

    status |= DEVICE_INITIALIZED;

    // Bring up fiber scheduler.
    scheduler_init(messageBus);

    for(int i = 0; i < DEVICE_COMPONENT_COUNT; i++)
    {
        if(CodalComponent::components[i])
            CodalComponent::components[i]->init();
    }

    // Seed our random number generator
    //seedRandom();

    // Create an event handler to trap any handlers being created for I2C services.
    // We do this to enable initialisation of those services only when they're used,
    // which saves processor time, memeory and battery life.
    messageBus.listen(DEVICE_ID_MESSAGE_BUS_LISTENER, DEVICE_EVT_ANY, this, &BLENano::onListenerRegisteredEvent);

    //codal_dmesg_set_flush_fn(nano_dmesg_flush);
    status |= DEVICE_COMPONENT_STATUS_IDLE_TICK;

    DMESG("start!");

    screen.init();
    screen.setAddrWindow(10, 10, 60, 70);
    for (uint32_t i = 0; i < sizeof(colors)/2; ++i) {
        colors[i] = color565(i % 10 < 5 ? 0xff0000 : 0x0000ff);
    }
    screen.sendColors(colors, sizeof(colors));
    memset(colors, 0xff, sizeof(colors));
    screen.sendColors(colors, sizeof(colors));

    return DEVICE_OK;
}

/**
  * A listener to perform actions as a result of Message Bus reflection.
  *
  * In some cases we want to perform lazy instantiation of components, such as
  * the compass and the accelerometer, where we only want to add them to the idle
  * fiber when someone has the intention of using these components.
  */
void BLENano::onListenerRegisteredEvent(Event evt)
{
}

/**
  * A periodic callback invoked by the fiber scheduler idle thread.
  * We use this for any low priority, backgrounf housekeeping.
  *
  */
void BLENano::idleCallback()
{
    codal_dmesg_flush();
}

void nano_dmesg_flush()
{
#if CONFIG_ENABLED(DMESG_SERIAL_DEBUG)
#if DEVICE_DMESG_BUFFER_SIZE > 0
    if (codalLogStore.ptr > 0 && ble_nano_device_instance)
    {
        for (uint32_t i=0; i<codalLogStore.ptr; i++)
            ((BLENano *)ble_nano_device_instance)->serial.putc(codalLogStore.buffer[i]);

        codalLogStore.ptr = 0;
    }
#endif
#endif
}
