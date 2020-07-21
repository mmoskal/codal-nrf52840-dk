
#ifndef BLUEBIRD_H
#define BLUEBIRD_H

#include "CodalHeapAllocator.h"
#include "codal-core/inc/types/Event.h"
#include "CodalDevice.h"
#include "ErrorNo.h"
#include "NRFLowLevelTimer.h"
#include "CodalCompat.h"
#include "CodalComponent.h"
#include "CodalDmesg.h"
#include "ManagedType.h"
#include "ManagedString.h"
#include "NotifyEvents.h"
#include "ZSingleWireSerial.h"

#include "Button.h"
#include "MultiButton.h"
#include "Timer.h"
#include "NRF52Pin.h"

#include "BLENanoIO.h"
#include "CodalFiber.h"
#include "MessageBus.h"

#include "NRF52Radio.h"



// Status flag values
#define DEVICE_INITIALIZED                    0x01

/**
 * Class definition for a BLENano device.
 *
 * Represents the device as a whole, and includes member variables that represent various device drivers
 * used to control aspects of the BLENano.
 */
namespace codal
{
    class Bluebird : public CodalComponent
    {
        private:

            /**
             * A listener to perform actions as a result of Message Bus reflection.
             *
             * In some cases we want to perform lazy instantiation of components, such as
             * the compass and the accelerometer, where we only want to add them to the idle
             * fiber when someone has the intention of using these components.
             */
            void onListenerRegisteredEvent(Event evt);

            // Pin ranges used for LED matrix display.

        public:

            BLENanoIO                   io;

            NRFLowLevelTimer            timer1;
            Timer                       timer;
            MessageBus                  messageBus;
            ZSingleWireSerial           sws;

            NRF52Radio                  radio;
            //Button                      buttonA;

            // Persistent key value store
            //BLENanoStorage           storage;

            // Bluetooth related member variables.
            //BLENanoBLEManager		  bleManager;
            //BLEDevice                   *ble;

            /**
             * Constructor.
             *
             * Create a representation of a Genuino Zero device, which includes member variables
             * that represent various device drivers used to control aspects of the board.
             */
            BLENano();

            /**
             * Post constructor initialisation method.
             */
            int init();

            /**
             * Delay execution for the given amount of time.
             *
             * If the scheduler is running, this will deschedule the current fiber and perform
             * a power efficient, concurrent sleep operation.
             *
             * If the scheduler is disabled or we're running in an interrupt context, this
             * will revert to a busy wait.
             *
             * Alternatively: wait, wait_ms, wait_us can be used which will perform a blocking sleep
             * operation.
             *
             * @param milliseconds the amount of time, in ms, to wait for. This number cannot be negative.
             *
             * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER milliseconds is less than zero.
             *
             */
            void sleep(uint32_t milliseconds);

            /**
             * A periodic callback invoked by the fiber scheduler idle thread.
             * We use this for any low priority, background housekeeping.
             */
            virtual void idleCallback();

            /**
             * Determine the time since this BLENano was last reset.
             *
             * @return The time since the last reset, in milliseconds.
             *
             * @note This will value overflow after 1.6 months.
             */
            //TODO: handle overflow case.
            unsigned long systemTime();
    };


    /**
     * Delay execution for the given amount of time.
     *
     * If the scheduler is running, this will deschedule the current fiber and perform
     * a power efficient, concurrent sleep operation.
     *
     * If the scheduler is disabled or we're running in an interrupt context, this
     * will revert to a busy wait.
     *
     * Alternatively: wait, wait_ms, wait_us can be used which will perform a blocking sleep
     * operation.
     *
     * @param milliseconds the amount of time, in ms, to wait for. This number cannot be negative.
     *
     * @return DEVICE_OK on success, DEVICE_INVALID_PARAMETER milliseconds is less than zero.
     *
     */
    inline void Bluebird::sleep(uint32_t milliseconds)
    {
        fiber_sleep(milliseconds);
    }

    /**
     * Determine the time since this BLENano was last reset.
     *
     * @return The time since the last reset, in milliseconds.
     *
     * @note This will value overflow after 1.6 months.
     */
    inline unsigned long Bluebird::systemTime()
    {
        return system_timer_current_time();
    }
}

void nano_dmesg_flush();

using namespace codal;

#endif
