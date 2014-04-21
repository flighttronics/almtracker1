/******************************************************************************

 errorhandler.c for Allmogetracker

 Allmogetracker receives data from a GPS receiver and transmits it
 over amature radio using the APRS protocol version 1.0.

 Copyright (C)2011, Andreas Kingbäck  (andki234@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
#include <avr/wdt.h>

#include "StdDefines.h"
#include "config.h"
#include "led.h"

#define CW_BASE_DELAY         (uint32_t)200000
#define CW_SHORT_DELAY        CW_BASE_DELAY * 1
#define CW_LONG_DELAY         CW_BASE_DELAY * 3
#define CW_LETTER_SPACE_DELAY CW_BASE_DELAY * 5
#define CW_WORD_SPACE_DELAY   CW_BASE_DELAY * 7

typedef enum
{
    NO_ERROR, NO_VALID_CALLSIGN
} T_ERROR;

typedef enum
{
    CW_DI, CW_DA
} T_DOT;

/******************************************************************************/
static void __ErrDelay(uint32_t delay)
/******************************************************************************/
{
    uint32_t i;

    for (i = 0; i < delay; i++)
    {
        wdt_reset(); // Reset watchdog timer
    }
}

/******************************************************************************/
static void __SendDot(T_DOT dot)
/******************************************************************************/
{
    switch (dot)
    {
        case CW_DI:
        {
            LED__Set(STATLED, ON); // Status LED on
            __ErrDelay(CW_SHORT_DELAY);
            LED__Set(STATLED, OFF); // Status LED off
            __ErrDelay(CW_SHORT_DELAY);

            break;
        }

        case CW_DA:
        {
            LED__Set(STATLED, ON); // Status LED on
            __ErrDelay(CW_LONG_DELAY);
            LED__Set(STATLED, OFF); // Status LED off
            __ErrDelay(CW_SHORT_DELAY);

            break;
        }
    }
}

/******************************************************************************/
static void __SendLEDError(void)
/******************************************************************************/
{
    // E
    __SendDot(CW_DI);
    __ErrDelay(CW_LETTER_SPACE_DELAY);

    // R
    __SendDot(CW_DI);
    __SendDot(CW_DA);
    __SendDot(CW_DI);
    __ErrDelay(CW_LETTER_SPACE_DELAY);

    // R
    __SendDot(CW_DI);
    __SendDot(CW_DA);
    __SendDot(CW_DI);
    __ErrDelay(CW_LETTER_SPACE_DELAY);

    // O
    __SendDot(CW_DA);
    __SendDot(CW_DA);
    __SendDot(CW_DA);
    __ErrDelay(CW_LETTER_SPACE_DELAY);

    // R
    __SendDot(CW_DI);
    __SendDot(CW_DA);
    __SendDot(CW_DI);
    __ErrDelay(CW_LETTER_SPACE_DELAY);
}

/******************************************************************************/
void ERROR_HANDLER__SignalError(void)
/******************************************************************************/
{
    LED__Set(STATLED, OFF); // Status LED off
	 
	 __ErrDelay(CW_WORD_SPACE_DELAY);

    if (!CONFIG__ValidCallsign())
    {
        __SendLEDError();
        __ErrDelay(CW_WORD_SPACE_DELAY);
    }
}
