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


#include "TeknikioDevice.h"
#include "Timer.h"
#include "nrf.h"

using namespace codal;

TeknikioDevice *teknikio_device_instance = NULL;

static const MatrixPoint ledMatrixPositions[5*6] =
{
    {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},
    {1,0},{1,1},{1,2},{1,3},{1,4},{1,5},
    {2,0},{2,1},{2,2},{2,3},{2,4},{2,5},
    {3,0},{3,1},{3,2},{3,3},{3,4},{3,5},
    {4,0},{4,1},{4,2},{4,3},{4,4},{4,5}
};

/**
  * Constructor.
  *
  * Create a representation of a TeknikioDevice device, which includes member variables
  * that represent various device drivers used to control aspects of the TeknikioDevice.
  */
TeknikioDevice::TeknikioDevice() :
    io(),
    timer1(NRF_TIMER1, TIMER1_IRQn),
    timer(timer1),
    messageBus(),
    sws(io.P30),
    radio(),
    ledRowPins{&io.P28, &io.P29, &io.P30},
    ledColPins{&io.P2, &io.P3, &io.P4},
    ledMatrixMap{ 5, 6, 3, 3, (NRF52Pin**)ledRowPins, (NRF52Pin**)ledColPins, ledMatrixPositions},
    display(ledMatrixMap)
{
    // Clear our status
    status = 0;
    teknikio_device_instance = this;

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

    // Configure serial port for debugging
    //serial.set_flow_control(mbed::Serial::Disabled);
    // serial.baud(115200);
}

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
int TeknikioDevice::init()
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
    messageBus.listen(DEVICE_ID_MESSAGE_BUS_LISTENER, DEVICE_EVT_ANY, this, &TeknikioDevice::onListenerRegisteredEvent);

#if CONFIG_ENABLED(DMESG_SERIAL_DEBUG)
#if DEVICE_DMESG_BUFFER_SIZE > 0
    codal_dmesg_set_flush_fn(nano_dmesg_flush);
#endif
#endif

    status |= DEVICE_COMPONENT_STATUS_IDLE_TICK;

    return DEVICE_OK;
}

/**
  * A listener to perform actions as a result of Message Bus reflection.
  *
  * In some cases we want to perform lazy instantiation of components, such as
  * the compass and the accelerometer, where we only want to add them to the idle
  * fiber when someone has the intention of using these components.
  */
void TeknikioDevice::onListenerRegisteredEvent(Event evt)
{
}

/**
  * A periodic callback invoked by the fiber scheduler idle thread.
  * We use this for any low priority, backgrounf housekeeping.
  *
  */
void TeknikioDevice::idleCallback()
{
#if DEVICE_DMESG_BUFFER_SIZE > 0
    codal_dmesg_flush();
#endif
}

void nano_dmesg_flush()
{
#if CONFIG_ENABLED(DMESG_SERIAL_DEBUG)
#if DEVICE_DMESG_BUFFER_SIZE > 0
    // if (codalLogStore.ptr > 0 && teknikio_device_instance)
    // {
    //     for (uint32_t i=0; i<codalLogStore.ptr; i++)
    //         ((TeknikioDevice *)teknikio_device_instance)->serial.putc(codalLogStore.buffer[i]);

    //     codalLogStore.ptr = 0;
    // }
#endif
#endif
}
