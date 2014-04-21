/******************************************************************************

 usart.c for Allmogetracker

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

#ifndef ENABLE_BIT_DEFINITIONS
#define ENABLE_BIT_DEFINITIONS
// Enable the bit definitions
#endif

// AVR specific includes

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// Application includes
#define _USART_C
#include "usart.h"
#include "led.h"
#include "stddefines.h"
#include "Messaging.h"
#undef _USART_C

/******************************************************************************/
void USART__Initialize(void)
/******************************************************************************
 * ABSTRACT:	This function initializes the usart
 *
 * INPUT   :	None
 * OUTPUT  :	None
 * RETURN  :	None
 */
{
    UBRRL = UBRR_VALUE; // See usart.h


    UCSRC = // Set to 8 data bits, no parity and 1stop bit
            (1 << URSEL) | // This bit selects between accessing the UCSRC or the UBRRH register.
                    (1 << UCSZ1) | // Bit 2:1 – UCSZ1:0: Character Size
                    (1 << UCSZ0); // 011 -> 8-bits

    UCSRB = (1 << RXCIE) | // Bit 7 – RXCIE: RX Complete Interrupt Enable
            (1 << TXEN) | // Bit 3 – TXEN: Transmitter Enable
            (1 << RXEN); // Bit 4 – RXEN: Receiver Enable

    return;

}

/******************************************************************************/
void USART__Flush(void)
/******************************************************************************
 * ABSTRACT:	This function flushes the hardware inbuffer
 *
 * INPUT   :	None
 * OUTPUT  :	None
 * RETURN  :	None
 */
{
    uint8_t dummy;

    while (UCSRA & (1 << RXC))
        dummy = UDR;
		
	(void)&dummy;		
}

/******************************************************************************/
void USART__SendByte(uint8_t data)
/******************************************************************************
 * ABSTRACT:	This function sends a byte
 *
 * INPUT   :	data to be sent
 * OUTPUT  :	None
 * RETURN  :	None
 */
{
    while ((UCSRA & (1 << UDRE)) == 0)
        asm("nop");
    if (UCSRA & (1 << UDRE)) // If the transmit buffer is empty
        UDR = data; // Place the byte in the output buffer

    return;
}

/******************************************************************************/
void USART__SendHexByte(uint16_t hex)
/******************************************************************************
 * ABSTRACT:    This function sends a byte in hex format
 *
 * INPUT   :    data to be sent
 * OUTPUT  :    None
 * RETURN  :    None
 */
{
    USART__SendByte((uint8_t)(hex >> 8));
    USART__SendByte((uint8_t)(hex & 0xFF));

    return;
}

/******************************************************************************/
void USART__SendString(const char *data)
/******************************************************************************
 * ABSTRACT:	This function sends a string
 *
 * INPUT   :	String to be sent
 * OUTPUT  :	None
 * RETURN  :	None
 */
{
    while (*data != 0) // While not to the string end yet
    {
        USART__SendByte(*(data++)); // Send the byte and increment index
    }

    return;
}

/******************************************************************************/
void USART__SendString_P(const char *data)
/******************************************************************************
 * ABSTRACT:	This function sends a string from program memory constant
 *
 * INPUT   :	String to be sent
 * OUTPUT  :	None
 * RETURN  :	None
 */
{
    while (pgm_read_byte(data) != 0)
    {
        USART__SendByte(pgm_read_byte(data++)); // Send the byte and increment index
    }

    return;
}

/******************************************************************************/
void USART__Execute(void)
/******************************************************************************
 * ABSTRACT:	This function transfers data from serial inbuffer to msg handler
 *
 * INPUT   :	String to be sent
 * OUTPUT  :	None
 * RETURN  :	None
 */
{
    if (in_tail != in_head)
    {
        if (++in_tail == BUFFER_SIZE)
            in_tail = 0;
        MsgHandler(in_buffer[in_tail]);
    }

    return;
}

/******************************************************************************/
SIGNAL(USART_RXC_vect)
/******************************************************************************
 * ABSTRACT:	Serial receive interrupt handler
 *
 * INPUT   :	None
 * OUTPUT  :	None
 * RETURN  :	None
 */
{
    LED__FlashStatusLED();

    if (++in_head == BUFFER_SIZE)
    {
        in_head = 0; // Advance and wrap buffer pointer
    }

    in_buffer[in_head] = UDR; // Transfer the byte to the input buffer

    return;
}

