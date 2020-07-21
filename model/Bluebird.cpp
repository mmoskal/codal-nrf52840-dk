
#include "Bluebird.h"
#include "Timer.h"
#include "nrf.h"

using namespace codal;

BLENano *bluebird_device_instance = NULL;

Bluebird::Bluebird() :
    io(),
    timer1(NRF_TIMER1, TIMER1_IRQn),
    timer(timer1),
    messageBus(),
    sws(io.P30),
    radio()
{
    
}
