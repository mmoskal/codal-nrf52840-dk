/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

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

/**
  * Class definition for BLENano IO.
  * Represents a collection of all I/O pins on the device.
  */

#include "CodalConfig.h"
#include "BLENanoIO.h"

using namespace codal;

/**
  * Constructor.
  *
  * Create a representation of all given I/O pins on the edge connector
  *
  * Accepts a sequence of unique ID's used to distinguish events raised
  * by NRF52Pin instances on the default EventModel.
  */
BLENanoIO::BLENanoIO() :
    P3 (ID_PIN_P3, P0_3, PIN_CAPABILITY_AD),
    P4 (ID_PIN_P4, P0_4, PIN_CAPABILITY_AD),
    P28 (ID_PIN_P28, P0_28, PIN_CAPABILITY_AD),
    P29 (ID_PIN_P29, P0_29, PIN_CAPABILITY_AD),
    P30 (ID_PIN_P30, P0_30, PIN_CAPABILITY_AD),
    P31 (ID_PIN_P31, P0_30, PIN_CAPABILITY_AD),

    led1 (ID_PIN_P13, P0_13, PIN_CAPABILITY_AD),
    led2 (ID_PIN_P14, P0_14, PIN_CAPABILITY_AD),
    led3 (ID_PIN_P15, P0_15, PIN_CAPABILITY_AD),
    led4 (ID_PIN_P16, P0_16, PIN_CAPABILITY_AD),

    spi (P3, *(Pin*)NULL, P4)
{
}
