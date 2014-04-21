/******************************************************************************
 delay.c for Allmogetracker

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

#include <stdint.h>
#include <avr/wdt.h>
#include "StdDefines.h"
#include "usart.h"

static uint8_t delay; // State of Delay function


/******************************************************************************/
void SetDelayState(uint8_t value)
{
    delay = value;
}

/******************************************************************************/
uint8_t GetDelayState(void)
{
    return delay;
}

/******************************************************************************/
void Delay(unsigned char timeout)
/*******************************************************************************
 * ABSTRACT:    This function sets "delay", programs the desired delay,
 *              and takes care of incoming serial characters until it's cleared.
 *
 * INPUT:       None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    delay = TRUE; // Set the condition variable
    wdt_reset(); // Kick the dog before we start
    TCNT2 = 255 - timeout; // Set desired delay
    while (delay)
    {
        // Do this until cleared by interrupt
        USART__Execute();
    }

    return;

} // End Delay(unsigned char timeout)

