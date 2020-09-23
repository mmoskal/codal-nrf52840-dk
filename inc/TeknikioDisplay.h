/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

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

#ifndef TEKNIKIO_DISPLAY_H
#define TEKNIKIO_DISPLAY_H

#include "CodalConfig.h"
#include "codal-core/inc/types/Event.h"
#include "LEDMatrix.h"
#include "AnimatedDisplay.h"
#include "NRF52Pin.h"
namespace codal
{

    struct MatrixMap52
    {
        int         width;                      // The physical width of the LED matrix, in pixels.
        int         height;                     // The physical height of the LED matrix, in pixels.
        int         rows;                       // The number of drive pins connected to LEDs.
        int         columns;                    // The number of sink pins connected to the LEDs.

        NRF52Pin    **rowPins;                  // Array of pointers containing an ordered list of pins to drive.
        NRF52Pin    **columnPins;               // Array of pointers containing an ordered list of pins to sink.

        const       MatrixPoint *map;           // Table mapping logical LED positions to physical positions.
    };
    /**
     * Class definition for TeknikioDisplay
     */
    class TeknikioDisplay //: public LEDMatrix, public AnimatedDisplay
    {
        public:

        /**
         * Constructor.
         *
         * Create a software representation of the micro:bit's 5x5 LED matrix.
         * The display is initially blank.
         *
         * @param map The mapping information that relates pin inputs/outputs to physical screen coordinates.
         * @param id The id the display should use when sending events on the MessageBus. Defaults to DEVICE_ID_DISPLAY.
         *
         */
        TeknikioDisplay(const MatrixMap52 &map, uint16_t id = DEVICE_ID_DISPLAY);

        /**
         * Destructor.
         */
        ~TeknikioDisplay();
    };
}

#endif