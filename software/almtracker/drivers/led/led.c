/******************************************************************************

 led.c for Allmogetracker

 Allmogetracker receives data from a GPS receiver and transmits it
 over amature radio using the APRS protocol version 1.0.

 Copyright (C)2010, Andreas Kingbäck  (andki234@gmail.com)

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

// OS headers
#include <avr/io.h>
#include <avr/interrupt.h>

#include "led.h"
// General purpose include files
#include "StdDefines.h"

static uint_least16_t led_on_in_ms = 0;

/******************************************************************************/
void LED__Init(void)
/******************************************************************************
 * ABSTRACT:    This function initializes the LED ports
 *
 * INPUT   :    None
 * OUTPUT  :    None
 * RETURN  :    None
 */
{
    // PORT C -
    //	Bit/Pin 2 (out) connected to the PTT circuitry
    //	Bit/Pin 4 (out) Status LED

    PORTC = 0x00; // Initial state is everything off
    DDRC  = 0x14; // Data direction register for port C
    led_on_in_ms = 0;
}

/******************************************************************************/
void LED__Set(int led, int state)
/******************************************************************************
 * ABSTRACT:    This function sets the LED state
 *
 * INPUT   :    led , state
 * OUTPUT  :    None
 * RETURN  :    None
 */
{
    switch (led)
    {
        case 1:
        {
            if (state == ON)
                PORTC |= 0x04;
            else
                PORTC &= ~(0X04);

            break;
        }
        case 2:
        {
            if (state == ON)
                PORTC |= 0x10;
            else
                PORTC &= ~(0X10);

            break;
        }
    }
}

/******************************************************************************/
void LED__FlashStatusLED(void)
/******************************************************************************
 * ABSTRACT:    This function flash the status led once
 *
 * INPUT   :    None
 * OUTPUT  :    None
 * RETURN  :    None
 */
{
    led_on_in_ms = 100;

    LED__Set(STATLED, ON); // Status LED on
}

/******************************************************************************/
void LED__DecrementLEDOnTime(void)
/******************************************************************************
 * ABSTRACT:    This function decrements the led on time
 *              Called every 100ms
 *
 * INPUT   :    None
 * OUTPUT  :    None
 * RETURN  :    None
 */
{
    if (led_on_in_ms >= 100)
    {
        led_on_in_ms -= 100;
    }
    else
    {
        LED__Set(STATLED, OFF); // Status LED OFF
    }
}

