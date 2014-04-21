/******************************************************************************
 tonegenerator.c for Allmogetracker

 Allmogetracker receives data from a GPS receiver and transmits it
 over amature radio using the APRS protocol version 1.0.

 Copyright (C)2010, Andreas Kingbäck  (andki234@gmail.com)
 Copyright (C)2005, Gary N. Dion (me@garydion.com).

 This module is based on Gary N. Dion (me@garydion.com) work.

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

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "ax25.h"
#include "delay.h"
#include "StdDefines.h"

static uint8_t transmit; // Keeps track of TX/RX state
static uint8_t usart_rx_flag; // State of mainDelay function

uint8_t TONEGENERATOR__GetTransmitStatus(void)
{
    return transmit;
}

void TONEGENERATOR__SetTransmitStatus(uint8_t tx)
{
    transmit = tx;
}

uint8_t TONEGENERATOR__GetUSARTRXFlag(void)
{
    return usart_rx_flag;
}

void TONEGENERATOR__SetUSARTRXFlag(uint8_t flag)
{
    usart_rx_flag = flag;
}


/******************************************************************************/
SIGNAL(TIMER0_OVF_vect)
/*******************************************************************************
 * ABSTRACT:    This routine now decodes packets by sampling the state of
 *              rxtoggled, and assembling a packet if toggles occur.  A checksum is
 *              performed for validation, then msg_end is set to message size.
 *
 * INPUT:       None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    if (transmit)
    {
        usart_rx_flag = FALSE; // Clear condition holding up mainDelay
        TCNT0 = 0; // Make long as possible delay
    };
} // End SIGNAL(SIG_OVERFLOW0)

/******************************************************************************/
SIGNAL(TIMER2_OVF_vect)
/*******************************************************************************
 * ABSTRACT:    This function handles the counter2 overflow interrupt.
 *              Counter2 is used to generate a sine wave using resistors on
 *              Pins B5-B1. Following are the sixteen 4-bit sinewave values:
 *                          7, 10, 13, 14, 15, 14, 13, 10, 8, 5, 2, 1, 0, 1, 2,
 *
 *       !!!Important!!! This code is -optimized- for the least # of clock cycles.
 *              If you modify it, PLEASE be sure you know what you're doing!
 *
 * INPUT:       None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    // This line is for if you followed the schematic:
    static char sine[16] =
    { 58, 22, 46, 30, 62, 30, 46, 22, 6, 42, 18, 34, 2, 34, 18, 42 };
    static unsigned char sine_index; // Index for the D-to-A sequence

    // PORT B - Sinewave Generation, and
    //  Bit/Pin 5 (out) connected to a 1k ohm resistor
    //  Bit/Pin 4 (out) connected to a 2k ohm resistor
    //  Bit/Pin 3 (out) connected to a 3.9k ohm resistor
    //  Bit/Pin 2 (out) connected to an 8.2k ohm resistor
    //  Bit/Pin 1 (out) connected to the PTT circuitry
    //  Bit/Pin 0 (out) DCD LED line


    if (transmit)
    {
        ++sine_index; // Increment index
        sine_index &= 15; // And wrap to a max of 15
        PORTB = sine[sine_index]; // Load next D-to-A sinewave value
        TCNT2 = Txtone; // Preload counter based on freq.
    }
    else
    {
        SetDelayState(FALSE); // Clear condition holding up Delay
        TCNT2 = 0; // Make long as possible delay
    }

} // End SIGNAL(SIG_OVERFLOW2)

