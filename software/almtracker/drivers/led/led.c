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

//-------------------------------------------------------------------------------------------------
//--------  I N C L U D E  ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"
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
    //	Bit/Pin 4 (out) GPS LED

    PORTC = 0x00; // Initial state is everything off
    DDRC = 0x14; // Data direction register for port C

#ifdef HW_VERSION_12

    // PORT D -
    //  Bit/Pin 5 (out) connected to baudrate indicator (ON = 19200)
    //  Bit/Pin 6 (out) Status LED

    PORTD = 0x00;// Initial state is everything off
    DDRD = 0x60;// Data direction register for port D
#endif

    led_on_in_ms = 0;
}

void LED__Power(int led, int state)
{
    switch (led)
    {
        case 1:
        {
            break;
        }

        case 2:
        {
            if (state == ON)
            {
                DDRD |= 0x10;
            }
            else
            {
                DDRD &= ~(0x10);
            }

            break;
        }

#ifdef HW_VERSION_12
        case 3:
        {
            if (state == ON)
            {
                DDRD |= 0x20;
            }
            else
            {
                DDRD &= ~(0x20);
            }

            break;
        }

        case 4:
        {
            if (state == ON)
            {
                DDRD |= 0x40;
            }
            else
            {
                DDRD &= ~(0x40);
            }

            break;
        }
#endif
    }
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
            {
                PORTC |= 0x04;
            }
            else
            {
                PORTC &= ~(0x04);
            }

            break;
        }

        case 2:
        {
            if (state == ON)
            {
                PORTC |= 0x10;
            }
            else
            {
                PORTC &= ~(0x10);
            }

            break;
        }

#ifdef HW_VERSION_12
            case 3:
            {
                if (state == ON)
                {
                    PORTD |= 0x20;
                }
                else
                {
                    PORTD &= ~(0x20);
                }

                break;
            }

            case 4:
            {
                if (state == ON)
                {
                    PORTD |= 0x40;
                }
                else
                {
                    PORTD &= ~(0x40);
                }

                break;
            }
#endif
    }
}

/******************************************************************************/
void LED__FlashGPSLED(void)
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

