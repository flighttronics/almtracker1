/******************************************************************************

 messaging.c for Allmogetracker

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

/**
 * @file messaging.c
 * NMEA functions
 * @brief This section contains functions to handle NMEA and convert it into MIC-E format
 *
 * @author Andreas Kingback
 * @version 0.00
 */

//-------------------------------------------------------------------------------------------------
//--------  I N C L U D E -------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <avr/wdt.h>

// General pure include files
#include "StdDefines.h"

// App required include files.
#include "ax25.h"
#include "Messaging.h"
#include "usart.h"
#include "led.h"
#include "config.h"
#include "time.h"

//-------------------------------------------------------------------------------------------------
//--------  D E F I N E S  ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define NMEABLEN 200 // NMEA buffer length
#define tempstrlen 200 // Tempstring lenght
typedef enum
{
    MSG_BIT_A, MSG_BIT_B, MSG_BIT_C, MSG_BIT_N_S, MSG_BIT_100, MSG_BIT_E_W
} T_LATITUDE_MESSAGE_BYTES;

//-------------------------------------------------------------------------------------------------
//--------  G L O B A L S  ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static char NMEAbuffer[NMEABLEN + 1]; // NMEA Buffer
static int NMEAindex; // NMEA Buffer index

static uint8_t sentence_type; // GPRMC, GPGGA, or unrecognized
static uint8_t NMEAStatus = INVALID; // We have not received any NMEA data
static uint8_t NoPacketsSent; // No packets that have been

const char hex[16] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

/******************************************************************************/
extern void MESSAGING__MsgInit(void)
/*******************************************************************************
 * ABSTRACT:	Initialize temp variables and set status
 *
 * INPUT:	None
 * OUTPUT:	None
 * RETURN:	None
 */
{
    NMEAStatus = INVALID; // We have not received any NMEA data
    NoPacketsSent = 0; // No packets sent yet

    return;
} // End MsgInit

/******************************************************************************/
uint16_t MESSAGING__Uint8ToHex(uint8_t value)
/*******************************************************************************
 * ABSTRACT:  Converts 8-bit uint to hex
 *
 * INPUT:   uint8
 * OUTPUT:  None
 * RETURN:  uint16
 */
{
    uint16_t hexval;

    hexval = hex[((value & 0xF0) >> 4)] << 8;
    hexval += hex[(value & 0x0F)];

    return hexval;
}

/******************************************************************************/
uint8_t HexToUint8(uint8_t hb, uint8_t lb)
/*******************************************************************************
 * ABSTRACT:  Converts 8-bit uint to hex
 *
 * INPUT:   uint8 , uint8
 * OUTPUT:  None
 * RETURN:  uint8
 */
{
    uint8_t hexval;

    if (hb >= 'A')
        hb += -55;
    else
        hb += -48;

    if (lb >= 'A')
        lb += -55;
    else
        lb += -48;

    hexval = (hb << 4) + lb;

    return hexval;
}

/******************************************************************************/
unsigned char NMEAChecksum(void)
/*******************************************************************************/
{
    uint8_t checksum;
    uint8_t i, j;

    checksum = 0;
    i = strlen(NMEAbuffer) - 3;
    for (j = 0; j < i; j++)
    {
        checksum ^= NMEAbuffer[j];
    }

    if ((hex[((checksum & 0xF0) >> 4)] == NMEAbuffer[i + 1]) & (hex[(checksum & 0x0F)] == NMEAbuffer[i + 2]))
        return TRUE;
    else
        return FALSE;

}
/******************************************************************************/
char ValidFix(void)
/*******************************************************************************
 * ABSTRACT:	Check that we have a valid GPS fix.
 *
 * INPUT:	None
 * OUTPUT:	None
 * RETURN:	VALID if we have a vailid GPS fix.
 */
{
    return NMEAStatus;
}

/******************************************************************************/
extern unsigned char strpos(char *str, char c, uint8_t maxlen)
/*******************************************************************************
 * ABSTRACT:	Find the possition in the NMEAbugffer after given no of , chars.
 *
 * INPUT:	Number of commas
 * OUTPUT:	None
 * RETURN:	Index possition in NMEAbuffer
 */
{
    uint8_t index;

    index = 0;
    while ((str[index] != c) && (index <= maxlen))
        index++;
    if (index > maxlen) // Set to 0 if not found
        index = 0;

    return index;
}

/******************************************************************************/
extern unsigned char GetNMEACommaPos(unsigned char c)
/*******************************************************************************
 * ABSTRACT:	Find the possition in the NMEAbugffer after given no of , chars.
 *
 * INPUT:	Number of commas
 * OUTPUT:	None
 * RETURN:	Index possition in NMEAbuffer
 */
{
    unsigned char commas, index;

    commas = 0;
    index = 0;
    while ((commas < c) && (index < NMEABLEN))
    {
        if (NMEAbuffer[index++] == ',')
            commas++;
    }
    return index;
}

/******************************************************************************/
extern unsigned char MsgPrepare_MICE(uint8_t astatus)
/*******************************************************************************/
{
    static char tempstr[tempstrlen + 1];
    uint8_t eeprom_data_buffer[EEPROM_DATA_SIZE];
    uint8_t checksum;
    uint16_t hex;
    int16_t i, temp;
    int16_t altitude, speed, track;
    int16_t index1, index2, a, b;
    uint8_t Status;
    uint16_t long_deg;
    uint8_t ns;
    uint8_t ew;

    Status = astatus;

    wdt_reset();
    // Reset watchdog timer

    MICEStr[0] = 0x27; // Set start byte for MIC-E

    // Determine packet type

    strncpy(tempstr, NMEAbuffer, 5);
    tempstr[5] = 0;

    if (0 == strcmp(tempstr, "GPGGA"))
    {
        sentence_type = GPGGA;
    }
    else if (0 == strcmp(tempstr, "GPRMC"))
    {
        sentence_type = GPRMC;
    }
    else if (0 == strcmp(tempstr, "PCAID"))
    {
        sentence_type = PCAID;
    }
    else if (0 == strcmp(tempstr, "EEPRE")) // cmd for sending the EEPROM data
    {
        sentence_type = SEND_EEPROM;
    }
    else if (0 == strcmp(tempstr, "EEPWR")) // cmd to receive and write data to EEPROM
    {
        sentence_type = WRITE_EEPROM;
    }
    else
    {
        return astatus; // Not a valid packet type return previous status
    }

    // Get NMEA time stamp
    // ------------------

    switch (sentence_type)
    {
        case SEND_EEPROM:
        {
            checksum = 0;

            EEPROM__ReadBlock(eeprom_data_buffer);

            for (i = 0; i < EEPROM_DATA_SIZE; i++)
            {
                hex = MESSAGING__Uint8ToHex(eeprom_data_buffer[i]);

                checksum ^= (uint8_t)(hex >> 8);
                checksum ^= (uint8_t) hex;

                USART__SendHexByte(hex);
            }

            hex = MESSAGING__Uint8ToHex(checksum);
            USART__SendByte('*');
            USART__SendHexByte(hex);

            break;
        }

        case WRITE_EEPROM:
        {
            index1 = GetNMEACommaPos(1); // Get index for first data

            for (i = 0; i < EEPROM_DATA_SIZE; i++)
            {
                eeprom_data_buffer[i] = HexToUint8(NMEAbuffer[index1],
                NMEAbuffer[index1 + 1]);

                index1 += 2;
            }

            EEPROM__WriteBlock(eeprom_data_buffer);

            USART__SendByte('O');
            USART__SendByte('K');

            break;
        }

        case GPGGA:
        case PCAID:
        {
            switch (sentence_type)
            {
                case GPGGA:
                {
                    index1 = GetNMEACommaPos(9); // Get pos for altitude
                    index2 = GetNMEACommaPos(10) - 1; // Get end pos for altitude

                    break;
                }

                case PCAID: // Barometer Altitude in meters (Leading zeros will be transmitted)

                {
                    index1 = GetNMEACommaPos(2); // Get pos for altitude gpsnav
                    index2 = GetNMEACommaPos(3) - 1; // Get end pos for altitude

                    break;
                }
            }

            // Get altitude
            // --------------

            i = 0;
            for (temp = index1; (temp < index2) && (i <= tempstrlen); temp++)
            {
                tempstr[i] = NMEAbuffer[temp];
                i++;
            }

            altitude = atof(tempstr);

            altitude += 10000;
            MICEStr[9] = 33 + (altitude / 8281);
            altitude %= 8281;
            MICEStr[10] = 33 + (altitude / 91);
            altitude %= 91;
            MICEStr[11] = 33 + altitude;
            MICEStr[12] = '}'; // End char for altitude

            switch (Status)
            // Update NMEAStatus

            {
                case VALID:
                {
                    break;
                }
                case OnlyGPRMC:
                {
                    Status = VALID;

                    break;
                }
                default:
                {
                    Status = OnlyGPGGA;

                    break;
                }
            }

            return Status;
        }

        case GPRMC:
        {
            // Get status
            // -------------

            // When the signals of at least three GPS satellites become stable,
            // the device can use the signals to calculate the current location.

            // A value of "A" (for "active") indicates that a fix is currently obtained,
            // whereas a value of "V" (for "inValid") indicates that a fix is not obtained.

            // $GPRMC,004345.00,A,5823.029,N,01606.026,E,87.1,205.3,270272,,*0F

            // The above value "A"  indicates that a fix is currently obtained.

            index1 = GetNMEACommaPos(2);

            if (NMEAbuffer[index1] != 'A')
            {
                return INVALID;
            }
            else
            {
                // Get N / S , E / W
                // ------------------
                index1 = GetNMEACommaPos(4);
                ns = NMEAbuffer[index1];

                index1 = GetNMEACommaPos(6);
                ew = NMEAbuffer[index1];

                // Get longitude Degrees
                // -----------------------
                index1 = GetNMEACommaPos(5);
                long_deg = ((NMEAbuffer[index1] - 48) * 100) + ((NMEAbuffer[index1 + 1] - 48) * 10) + (NMEAbuffer[index1 + 2] - 48);

                // Get latitude
                // ------------
                index1 = GetNMEACommaPos(3);

                for (index2 = 0; index2 < 6; index2++)
                {
                    switch (index2)
                    {
                        case MSG_BIT_A:
                        case MSG_BIT_B:
                        {
                            DestAddress[index2] = NMEAbuffer[index1++] + 32;

                            break;
                        }

                        case MSG_BIT_C:
                        {
                            DestAddress[index2] = NMEAbuffer[index1++];

                            break;
                        }

                        case MSG_BIT_N_S:
                        {
                            if (ns == 'N')
                            {
                                DestAddress[index2] = NMEAbuffer[index1++] + 32;
                            }
                            else
                            {
                                DestAddress[index2] = NMEAbuffer[index1++];
                            }

                            index1++; // Skip '.'

                            break;
                        }

                        case MSG_BIT_100:
                        {
                            if ((long_deg < 10) || ((long_deg >= 100) && (long_deg <= 179)))
                            {
                                DestAddress[index2] = NMEAbuffer[index1++] + 32;
                            }
                            else
                            {
                                DestAddress[index2] = NMEAbuffer[index1++];
                            }
                            break;
                        }

                        case MSG_BIT_E_W:
                        {
                            if (ew == 'W')
                            {
                                DestAddress[index2] = NMEAbuffer[index1++] + 32;
                            }
                            else
                            {
                                DestAddress[index2] = NMEAbuffer[index1++];
                            }

                            break;
                        }
                    }
                }

                // Get longitude
                // --------------

                // Longitude Degrees Encoding

                index1 = GetNMEACommaPos(5);

                if (long_deg < 10) // 0ï¿½9 degrees: d+28 is in the range 118ï¿½127 decimal,

                {
                    MICEStr[1] = long_deg + 118; // corresponding to the ASCII characters v to DEL.
                }
                else
                { // Important Note: The longitude offset is set to +100
                  // degrees when the longitude is in the range 0ï¿½9 degrees.
                    if (long_deg < 100) // 10ï¿½99 degrees: d+28 is in the range 38ï¿½127 decimal

                    {
                        MICEStr[1] = long_deg + 28; // (corresponding to the ASCII characters & to DEL), and
                    }
                    else // the longitude offset is +0 degrees.
                    {
                        if (long_deg < 110) // 100ï¿½109 degrees: d+28 is in the range 108ï¿½117 decimal,'

                        {
                            MICEStr[1] = long_deg + 8; // (corresponding to the ASCII characters l to   DEL), and
                        }
                        else
                        {
                            // the longitude offset is +100 degrees.
                            MICEStr[1] = long_deg - 72; // 110ï¿½179 degrees: d+28 is in the range 38ï¿½127 decimal
                            // (corresponding to the ASCII characters & to DEL), and
                            // the longitude offset is +100 degrees
                        }
                    }
                }

                // Longitude Minutes Encoding

                index1 += 3;

                temp = ((NMEAbuffer[index1] - 48) * 10) + (NMEAbuffer[index1 + 1] - 48);

                if (temp < 0)
                {
                    temp = 0;
                }

                if (temp < 10) // 0ï¿½9 minutes: m+28 is in the range 88ï¿½97 decimal,

                {
                    MICEStr[2] = temp + 88; // (corresponding to the ASCII characters X to a).
                }
                else
                {
                    MICEStr[2] = temp + 28; // 10ï¿½59 minutes: m+28 is in the range 38ï¿½87 decimal
                    // (corresponding to the ASCII characters & to W).
                }

                index1 += 3; // Skip '.'

                // Longitude Hundredths of Minutes Encoding

                temp = ((NMEAbuffer[index1] - 48) * 10) + (NMEAbuffer[index1 + 1] - 48);

                if (temp < 0)
                {
                    temp = 0;
                }

                MICEStr[3] = temp + 28; // The h+28 byte in the Information field contains the encoded
                // value of the longitude hundredths of minutes, in the range
                // 0ï¿½99 minutes. This byte takes a value in the range 28 decimal
                // (corresponding to 0 hundredths of a minute) through 127 decimal
                // (corresponding to 99 hundredths of a minute).
                // Get heading and speed
                // ---------------------

                index1 = GetNMEACommaPos(7); // Get speed
                index2 = GetNMEACommaPos(8);
                i = 0;
                for (temp = index1; (temp < index2) && (i <= tempstrlen); temp++)
                {
                     tempstr[i] = NMEAbuffer[temp];
                    i++;
                }

                speed = atof(tempstr);

                index1 = GetNMEACommaPos(8); // Get track
                index2 = GetNMEACommaPos(9);
                i = 0;
                for (temp = index1; (temp < index2) && (i <= tempstrlen); temp++)
                {
                    tempstr[i] = NMEAbuffer[temp];
                    i++;
                }

                track = atof(tempstr);
                index1 = 48;

                if (track == 0)
				{
                    track = 360;
				}				

                MICEStr[4] = (speed < 200) ? 108 + (speed / 10) : 48 + ((speed - 200) / 10);

                a = speed - ((speed / 10) * 10);
                MICEStr[5] = 32 + (10 * a) + (track / 100);

                b = track - ((track / 100) * 100);
                MICEStr[6] = 28 + b;
				
                MICEStr[7] = 'g'; // Small Aircraft
                MICEStr[8] = '/';

                // Update Status
                switch (Status)
                {
                    case VALID:
                    {
                        break;
                    }
                    case OnlyGPGGA:
                    {
                        Status = VALID;

                        break;
                    }
                    default:
                    {
                        Status = OnlyGPRMC;

                        break;
                    }
                }

                return Status;
            }
        }
    }

    wdt_reset(); // Reset watchdog timer

    return Status;
}

/******************************************************************************/
extern void MsgSendBeacon(void)
/*******************************************************************************
 * ABSTRACT:
 *
 * INPUT:	None
 * OUTPUT:	None
 * RETURN:	None

 */
{
    // Fix this!! a quick fix....

    char beacon[50];

    NoPacketsSent++;

    if ((NoPacketsSent % 10) == 0)
    {
        CONFIG__GetBeacon(beacon, VERBEACON);
    }
    else if ((NoPacketsSent % 5) == 0)
    {
        CONFIG__GetBeacon(beacon, OPBEACON);
    }
    else
    {
        return;
    }

    wdt_reset(); // Reset watch dog timer

    AX25__SendString(beacon);

    return;
}

/******************************************************************************/
extern void MsgSendPos(void)
/*******************************************************************************
 * ABSTRACT:	Send position in MICE-format
 *
 * INPUT:	None
 * OUTPUT:	None
 * RETURN:	None
 */
{
    unsigned char i;

    for (i = 0; i < 13; i++)
        AX25__SendByte(MICEStr[i]);

    wdt_reset();
    // Reset watchdog timer

} // End MsgSendPos(void)

/******************************************************************************/
extern void MsgHandler(unsigned char newchar)
/*******************************************************************************
 * ABSTRACT:	Processes the characters coming in from USART.  In this case,
 *				this is the port connected to the gps receiver.
 *
 * INPUT:		newchar	Next character from the serial port.
 * OUTPUT:	None
 * RETURN:	None
 */
{
    uint8_t NMEABufferValid;

    if (newchar == 0) // A NULL character resets GPS decoding
    {
        NMEAStatus = INVALID; // We have not received a complete GPS fix
        return;
    }

    if ((newchar == '$') || (newchar == '!')) // Start of Sentence character, reset
    {
        NMEAindex = 0; // Reset NMEABuffer length
        return;
    }

    if (newchar == 10) // Don't add char 10 to buffer
        return;

    if (newchar == 13)
    {
        NMEAbuffer[NMEAindex] = 0;
        NMEABufferValid = NMEAChecksum(); // Check if NMEA string is valid

        if (NMEABufferValid == TRUE)
            NMEAStatus = MsgPrepare_MICE(NMEAStatus);
        return;
    }

    if (NMEAindex > NMEABLEN - 1) // Reset if buffer overflow
    {
        NMEAindex = 0; // Reset NMEABuffer length
        return;
    }

    NMEAbuffer[NMEAindex++] = newchar;

    return;

} // End MsgHandler(unsigned char newchar)
