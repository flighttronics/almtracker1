/******************************************************************************
 ax25.c for Allmogetracker

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

// Important! One global variable is used in this file: Txtone

// OS headers
#include <avr/eeprom.h>
#include <avr/io.h>

// General purpose include files
#include "StdDefines.h"

// App required include files
#include "ax25.h"
#include "Main.h"
#include "usart.h"
#include "config.h"

// Defines
#define BIT_DELAY 189					// Delay for 0.833 ms (189 for 14.7456 MHz)
#define TXDELAY 200						// Tx delay in ms
// Global variables
static unsigned short crc;

/******************************************************************************/
extern void AX25__SendHeader(void)
/*******************************************************************************
 * ABSTRACT:	This function keys the transmitter, sends the source and
 *				destination address, and gets ready to send the actual data.
 *
 * INPUT:	None
 * OUTPUT:	None
 * RETURN:	None
 */
{
    unsigned char loop_delay;

    crc = 0xFFFF; // Initialize the crc register

    // Transmit the Flag field to begin the UI-Frame
    // Adjust length for txdelay
    // Transmit the Flag field to begin the UI-Frame

    // Transmit the Flag field to begin the UI-Frame
    // Adjust length for txdelay (each one takes 6.7ms)
    for (loop_delay = 0; loop_delay < TXDELAY; loop_delay++)
    {
        AX25__SendByte(0x7E);
    }

    /* 		* * * THIS IS WHERE THE CALLSIGNS ARE DETERMINED * * *
     Each callsign character is shifted to use the high seven bits of the byte.
     Use the table below to determine the hex values for these characters.
     For callsigns less than six digits, pad the end of the callsign with spaces.
     For all bytes in header, except for the very last byte, bit0 must be clear.
     Yes, this means only the very last byte has bit0 set to 1 in the Station ID!

     Callsign byte lookup table
     -----------------------------------------------------------
     | Letters:                    |   Numbers and SSID's:       |
     | A = 0x82       N = 0x9C     |   0 = 0x60       8 = 0x70   |
     | B = 0x84       O = 0x9E     |   1 = 0x62       9 = 0x72   |
     | C = 0x86       P = 0xA0     |   2 = 0x64       10 = 0x74  |
     | D = 0x88       Q = 0xA2     |   3 = 0x66       11 = 0x76  |
     | E = 0x8A       R = 0xA4     |   4 = 0x68       12 = 0x78  |
     | F = 0x8C       S = 0xA6     |   5 = 0x6A       13 = 0x7A  |
     | G = 0x8E       T = 0xA8     |   6 = 0x6C       14 = 0x7C  |
     | H = 0x90       U = 0xAA     |   7 = 0x6E       15 = 0x7E  |
     | I = 0x92       V = 0xAC     |   Space = 0x40              |
     | J = 0x94       W = 0xAE     |                             |
     | K = 0x96       X = 0xB0     |   REMEMBER!  Set bit0 in    |
     | L = 0x98       Y = 0xB2     |   the last SSID to one!     |
     | M = 0x9A       Z = 0xB4     |                             |
     -----------------------------------------------------------
     End of lookup table */

    // Begin transmission of packet destination address (APxxxx0)

    AX25__SendDestination(); // Begin transmission of packet source address

    AX25__SendCallsign(); // Send Callsign from eeprom

    AX25__SendPath(); // Send Path

    // Finish out the header with two more bytes
    AX25__SendByte(0x03); // Control field - 0x03 is APRS UI-frame
    AX25__SendByte(0xF0); // Protocol ID - 0xF0 is no layer 3

    return;

} // End AX25__sendHeader(void)

extern void AX25__SendPath(void)
{
    char wide[5] =
    {0xAE, 0x92, 0x88, 0x8A, 0x00};

    uint8_t path = CONFIG__GetPath();

    switch (path)
    {
        case 0:
        default:
        {
            AX25__SendString(wide); // WIDE
            AX25__SendByte(0x62); // Byte 5 (1)
            AX25__SendByte(0x40); // Byte 6 (Space)
            AX25__SendByte(0x63); // Station ID (1)

            break;
        }

        case 1:
        {
            AX25__SendString(wide); // WIDE
            AX25__SendByte(0x62); // Byte 5 (1)
            AX25__SendByte(0x40); // Byte 6 (Space)
            AX25__SendByte(0x62); // Station ID (1)
            AX25__SendString(wide); // WIDE
            AX25__SendByte(0x64); // Byte 5 (2)
            AX25__SendByte(0x40); // Byte 6 (Space)
            AX25__SendByte(0x65); // Station ID (2)
        }
    }
}

/******************************************************************************/
extern void AX25__SendFooter(void)
/*******************************************************************************
 * ABSTRACT:	This function closes out the packet with the check-sum and a
 *				final flag.
 *
 * INPUT:		None
 * OUTPUT:	None
 * RETURN:	None
 */
{
    static unsigned char crchi;

    crchi = (crc >> 8) ^ 0xFF;
    AX25__SendByte(crc ^ 0xFF); // Send the low byte of the crc
    AX25__SendByte(crchi); // Send the high byte of the crc
    AX25__SendByte(0x7E); // Send a flag to end the packet
    return;

    PORTC &= ~(0xFF);

} // End ax25sendFooter(void)


/******************************************************************************/
extern void AX25__SendByte(unsigned char txbyte)
/*******************************************************************************
 * ABSTRACT:	This function sends one byte by toggling the "tone" variable.
 *
 * INPUT:		txbyte	The byte to transmit
 * OUTPUT:	None
 * RETURN:	None
 */
{
    static char loop;
    static char bitbyte;
    static int bit_zero;
    static unsigned char sequential_ones;

    bitbyte = txbyte; // Bitbyte will be rotated through

    for (loop = 0; loop < 8; loop++) // Loop for eight bits in the byte
    {
        bit_zero = bitbyte & 0x01; // Set aside the least significant bit

        if (txbyte == 0x7E) // Is the transmit character a flag?
        {
            sequential_ones = 0; // it is immune from sequential 1's
        }
        else // The transmit character is not a flag
        {
            (AX25__CrcBit(bit_zero)); // So modify the checksum
        }

        if (!(bit_zero)) // Is the least significant bit low?
        {
            sequential_ones = 0; // Clear the number of ones we have sent
            Txtone = (Txtone == MARK) ? SPACE : MARK; // Toggle transmit tone
        }
        else // Else, least significant bit is high
        {
            if (++sequential_ones == 5) // Is this the 5th "1" in a row?
            {
                mainDelay(BIT_DELAY); // Go ahead and send it
                Txtone = (Txtone == MARK) ? SPACE : MARK; // Toggle transmit tone
                sequential_ones = 0; // Clear the number of ones we have sent
            }

        }

        bitbyte >>= 1; // Shift the reference byte one bit right
        mainDelay(BIT_DELAY); // Pause for the bit to be sent
    }

    return;

} // End AX25__SendByte(unsigned char txbyte)


/******************************************************************************/
extern void AX25__CrcBit(int lsb_int)
/*******************************************************************************
 * ABSTRACT:	This function takes the latest transmit bit and modifies the crc.
 *
 * INPUT:		lsb_int	An integer with its least significant bit set of cleared
 * OUTPUT:	None
 * RETURN:	None
 */
{
    static unsigned short xor_int;

    xor_int = crc ^ lsb_int; // XOR lsb of CRC with the latest bit
    crc >>= 1; // Shift 16-bit CRC one bit to the right

    if (xor_int & 0x0001) // If XOR result from above has lsb set
    {
        crc ^= 0x8408; // Shift 16-bit CRC one bit to the right
    }

    return;

} // End AX25__CrcBit(int lsb_int)


/******************************************************************************/
extern void AX25__SendString(char *address)
/*******************************************************************************
 * ABSTRACT:	This function sends a null-terminated string to the packet
 *
 * INPUT:		*szString	Pointer to string to send
 * OUTPUT:	None
 * RETURN:	None
 */
{
    while (*address != 0)
    {
        AX25__SendByte(*address);
        address++;
    }

    return;

} // End AX25__sendString(char *address)


/******************************************************************************/
extern void AX25__SendDestination()
/*******************************************************************************
 * ABSTRACT:	This function sends callsign string found in EEPROM
 *
 *
 * INPUT:	None
 * OUTPUT:	None
 * RETURN:	None
 */
{
    unsigned char i;

    for (i = 0; i < 6; i++)
        AX25__SendByte((DestAddress[i] << 1) & 0xFE);

    AX25__SendByte(0x60); // SSID=0
}

/******************************************************************************/
extern void AX25__SendCallsign()
/*******************************************************************************
 * ABSTRACT:	This function sends callsign string found in EEPROM
 *
 *
 * INPUT:	None
 * OUTPUT:	None
 * RETURN:	None
 */
{
    char callsign[7];
    int i;

    CONFIG__GetCallsign(callsign);

    for (i = 0; i < 7; i++)
        AX25__SendByte(callsign[i]);

    return;

} // End AX25__SendCallsign(void)


