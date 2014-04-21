/******************************************************************************
 system.c for Allmogetracker

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

#include <avr/wdt.h>
#include "system.h"
#include "usart.h"
#include "config.h"
#include "eeprom.h"
#include "version.h"
#include "led.h"
#include "messaging.h"

/**
 * Configures system timers
 */

/******************************************************************************/
void SYSTEM__SetupTimers(void)
/*******************************************************************************/
{
    //  Initialize the 8-bit Timer0
    TCCR0 = 0x03; // Timer0 clock prescale of 64

    // Use the 16-bit Timer1 to count to tx interval. Set it to clock at 14400 Hz
    TCCR1B = 0x05; // Timer1 clock prescale of 1024

    //  Initialize the 8-bit Timer2
    TCCR2 = 0x02; // Timer2 clock prescale of 8

    // Enable Timer interrupts
    TIMSK = 1 << TOIE0 | 1 << TOIE2 | 1 << TOIE1;
}

/**
 * Configures system ports
 */

/******************************************************************************/
void SYSTEM__SetupPorts(void)
/*******************************************************************************/
{
    PORTB = 0x00; // Initial state is everything off
    DDRB = 0x3F; // Data direction register for port B
}

/**
 * Watchdog timer setup function
 */
/*******************************************************************************/
void SYSTEM__InitWatchDog(void)
/*******************************************************************************/
{

    WDTCR = (1 << WDCE) | (1 << WDE); // Wake-up the watchdog register
    WDTCR = (1 << WDE) | 7; // Enable and timeout around 2.1s

    wdt_reset(); // Reset watchdog timer
}

/**
 * Calls all system init functions
 * @see LED__Init()
 * @see USART__Initialize()
 * @see MsgInit()
 */
/******************************************************************************/
void SYSTEM__InitSystem(void)
/*******************************************************************************/
{
	SYSTEM__SetupPorts();

    SYSTEM__SetupTimers();

    LED__Init(); // Init LED
	
	CONFIG__CheckEEPROM();

    USART__Initialize(); //Initialize serial communication functions

    MESSAGING__MsgInit();
}

/**
 * Transmits system information to USART
 * @see USART__SendString_P()
 */

/******************************************************************************/
void SYSTEM__SendSystemInfoToUSART(void)
/*******************************************************************************/
{
    USART__SendString_P(firm_version_P);
    USART__SendString_P(ready_P);
}
