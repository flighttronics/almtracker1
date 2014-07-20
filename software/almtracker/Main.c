/******************************************************************************

 main.c for Allmogetracker

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

// OS headers
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

// General purpose include files
#include "StdDefines.h"

// App required include files
#include "tonegenerator.h"
#include "ax25.h"
#include "main.h"
#include "delay.h"
#include "messaging.h"
#include "system.h"
#include "usart.h"
#include "config.h"
#include "led.h"
#include "version.h"
#include "errorhandler.h"

#define	RXSIZE (256)

// Static functions and variables

static uint16_t seconds_to_next_transmit; // A failsafe to

/******************************************************************************/
static void _StartTransmiting(void)
/*******************************************************************************
 * ABSTRACT:    Do all the setup to transmit.
 *
 * INPUT:       None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    UCSRB &= ~(1 << RXCIE); // Disable the serial RX interrupt

    TONEGENERATOR__SetTransmitStatus(TRUE); // Enable the transmitter

    LED__Set(TXLED, ON); // PTT and TXLED on

    wdt_reset(); // Reset watchdog timer

    AX25__SendHeader(); // Send APRS header
}

/******************************************************************************/
static void _EndTransmit(void)
/*******************************************************************************
 * ABSTRACT:    End Transmit cycle
 *
 * INPUT:   None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    AX25__SendFooter(); // Send APRS footer

    TONEGENERATOR__SetTransmitStatus(FALSE); // Disable transmitter

    LED__Set(TXLED, OFF); // PTT off and TXLED off

    UCSRB |= (1 << RXCIE); // Re-enable the RX serial interrupt

    MsgHandler(0); // Reset the GPS decoding engine

    wdt_reset(); // Reset watchdog timer
}

/******************************************************************************/
extern void mainDelay(unsigned char timeout)
/*******************************************************************************
 *
 * ABSTRACT:    This function sets "maindelay", programs the desired delay,
 *              and takes care of incoming serial characters until it's cleared.
 *
 * INPUT:   None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    TONEGENERATOR__SetUSARTRXFlag(TRUE); // Set the condition variable

    wdt_reset(); // Reset watchdog timer

    TCNT0 = 255 - timeout; // Set desired delay

    while (TONEGENERATOR__GetUSARTRXFlag())
    {
        USART__Execute(); // Do this until cleared by interrupt
    }

    return;

}

/******************************************************************************/
extern void DecrementTimeToTransmit(void)
/*******************************************************************************
 *
 * ABSTRACT:    This function decrements the seconds_to_next_transmit variable
 *
 * INPUT:   None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    if (seconds_to_next_transmit > 0)
    {
        seconds_to_next_transmit--;
    }
}

/**
 * Main function
 * @see InitWatchDog()
 * @see InitSystem()
 * @see SendSystemInfoToUSART();
 * @see SetupPorts()
 * @see SetupTimers()
 * @see Delay()
 * @see ValidFix()
 * @see LED__Set()
 * @see mainTransmit()
 * @see MsgSendPos()
 * @see MsgSendBeacon()
 * @see mainReceive()

 */

/******************************************************************************/
int main(void)
/*******************************************************************************/
{
    SYSTEM__InitWatchDog();
    SYSTEM__InitSystem();

    SYSTEM__SendSystemInfoToUSART();

#ifdef HW_VERSION_12
    if (CONFIG__GetBaud() == BUAD_19200)
    {
        LED__Set(BAUDLED,ON);
    }
#endif


    ERROR_HANDLER__SignalError(); // Signal ERROR or OK if CALLSIGN is corrupt

#ifdef HW_VERSION_12
    LED__Power(BAUDLED, OFF);
    LED__Power(STATLED, OFF);
#endif
    // Enable interrupts
    sei();

    MsgHandler(0); // Reset the GPS decoding engine

    // On reset minimum transmit time is 1min

    seconds_to_next_transmit = 60;

    // Set sleep mode
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    while (1 == 1)
    {
        Delay(250);

        wdt_reset(); // Reset watchdog timer

        if ((ValidFix() == VALID) && (CONFIG__ValidCallsign())) // Valid fix and valid callsign
        {
            LED__Set(GPSLED, ON); // Status LED on

            if (seconds_to_next_transmit == 0)
            {
                wdt_reset(); // Reset watchdog timer
                _StartTransmiting(); // Enable transmitter
                MsgSendPos(); // Send Position Report
                MsgSendBeacon(); // Send Beacon
                seconds_to_next_transmit = CONFIG__GetTXRate(); // x sec to next packet
                _EndTransmit(); // Disable transmitter
            }
        }
        else
        {
            LED__Set(GPSLED, OFF); // Status LED off
            sleep_cpu();
        }
    }

    return (1);
}

