/******************************************************************************
 nmea.c for Allmogetracker 2

 Allmogetracker receives data from a GPS receiver and transmits it
 over amature radio using the APRS protocol version 1.0.

 Copyright (C)2011, Andreas Kingb�ck  (andki234@gmail.com)

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
 * @file nmea.c
 * NMEA functions
 * @brief This section contains functions to handle NMEA data from the GPS
 *
 * @author Andreas Kingb�ck
 * @version 0.00
 */

//-------------------------------------------------------------------------------------------------
//--------  I N C L U D E -------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "nmea.h"

//-------------------------------------------------------------------------------------------------
//--------  D E F I N E S  ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define NMEA_BUFFER_LENGTH 100
#define MICE_BUFFER_LENGTH 13

typedef enum
{
    MSG_BIT_A,
    MSG_BIT_B,
    MSG_BIT_C,
    MSG_BIT_N_S,
    MSG_BIT_100,
    MSG_BIT_E_W
} T_LATITUDE_MESSAGE_BYTES;

//-------------------------------------------------------------------------------------------------
//--------  G L O B A L S  ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
const char hex[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static uint8_t nmea_buffer[NMEA_BUFFER_LENGTH];
static uint8_t mice_buffer[MICE_BUFFER_LENGTH];
static uint8_t destination_address[7];
static T_NMEA_STATUS nmea_status = INVALID;

//-------------------------------------------------------------------------------------------------
//--------  L O C A L  F U N C T I O N S ----------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static T_NMEA_COMMANDS _GetSentenceType(uint8_t *nmea)
{
    char tempstr[6];

    strncpy(tempstr, (char *) nmea + 1, 5);
    tempstr[5] = 0;

    if (strcmp(tempstr, "GPGGA") == 0)
    {
        return GPGGA;
    }
    else if (strcmp(tempstr, "GPRMC") == 0)
    {
        return GPRMC;
    }
    else if (strcmp(tempstr, "PCAID") == 0)
    {
        return PCAID;
    }
    else
    {
        return UKNOWN;
    }
}

static uint8_t _GetNMEACommaPos(uint8_t *nmea, uint8_t c)
{
    unsigned char commas, index;

    commas = 0;
    index = 0;

    while ((commas < c) && (index < NMEA_BUFFER_LENGTH))
    {
        if (nmea[index++] == ',')
            commas++;
    }

    return index;
}

bool _isNMEAStringValid(uint8_t *nmea)
{
    uint8_t checksum;
    uint8_t i, j;

    checksum = 0;

    i = strlen((char*) nmea) - 3;

    if (i > NMEA_BUFFER_LENGTH)
    {
        i = NMEA_BUFFER_LENGTH;
    }

    for (j = 1; j < i; j++)
    {
        checksum ^= nmea[j];
    }

    if ((hex[((checksum & 0xF0) >> 4)] == nmea[i + 1]) & (hex[(checksum & 0x0F)] == nmea[i + 2]))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------------------------
//--------  G L O B A L  F U N C T I O N S --------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 This function converts NMEA to MICE format
 */bool NMEA__NMEAToMICE(uint8_t *nmea, uint8_t *mice, T_NMEA_STATUS *nmea_status, uint8_t *destination_address)
{
    char tempstr[NMEA_BUFFER_LENGTH]; // TODO Static?
    char ns;
    char ew;
    int i, temp;
    int altitude, speed, track;
    int index1, index2, a, b;
    int long_deg;
    uint8_t sentence_type;

    mice[0] = 0x27; // Set start byte for MIC-E

    // Get NMEA time stamp
    // ------------------

    sentence_type = _GetSentenceType(nmea);

    switch (sentence_type)
    {
        case GPGGA:
        case PCAID:
        {
            switch (sentence_type)
            {
                case GPGGA:
                {
                    index1 = _GetNMEACommaPos(nmea, 9); // Get pos for altitude
                    index2 = _GetNMEACommaPos(nmea, 10) - 1; // Get end pos for altitude

                    break;
                }

                case PCAID: // Barometer Altitude in meters (Leading zeros will be transmitted)
                {
                    index1 = _GetNMEACommaPos(nmea, 2); // Get pos for altitude gpsnav
                    index2 = _GetNMEACommaPos(nmea, 3) - 1; // Get end pos for altitude

                    break;
                }
            }

            // Get altitude
            // --------------

            i = 0;
            for (temp = index1; (temp < index2) && (i <= NMEA_BUFFER_LENGTH); temp++)
            {
                tempstr[i] = nmea[temp];
                i++;
            }

            altitude = (int) atof(tempstr);

            altitude += 10000;
            mice[9] = 33 + (altitude / 8281);
            altitude %= 8281;
            mice[10] = 33 + (altitude / 91);
            altitude %= 91;
            mice[11] = 33 + altitude;
            mice[12] = '}'; // End char for altitude

            switch (*nmea_status)
            {
                case VALID:
                {
                    break;
                }
                case ONLY_GPRMC:
                {
                    *nmea_status = VALID;

                    break;
                }
                default:
                {
                    *nmea_status = ONLY_GPGGA;

                    break;
                }
            }

            return false;
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

            index1 = _GetNMEACommaPos(nmea, 2);

            if (nmea[index1] != 'A')
            {
                return INVALID;
            }
            else
            {
                // Get time
                // ------------
                index1 = _GetNMEACommaPos(nmea, 1);

                // Get date
                // ------------
                index1 = _GetNMEACommaPos(nmea, 9);

                // Get N / S , E / W
                // ------------------
                index1 = _GetNMEACommaPos(nmea, 4);
                ns = nmea[index1];

                index1 = _GetNMEACommaPos(nmea, 6);
                ew = nmea[index1];

                // Get longitude Degrees
                // -----------------------
                index1 = _GetNMEACommaPos(nmea, 5);
                long_deg = ((nmea[index1] - 48) * 100) + ((nmea[index1 + 1] - 48) * 10) + (nmea[index1 + 2] - 48);

                // Get latitude
                // ------------
                index1 = _GetNMEACommaPos(nmea, 3);

                for (index2 = 0; index2 < 6; index2++)
                {
                    //printf("nmea[%d] = %c\n", index1, nmea[index1]);

                    switch (index2)
                    {
                        case MSG_BIT_A:
                        case MSG_BIT_B:
                        {
                            destination_address[index2] = nmea[index1++] + 32;

                            break;
                        }

                        case MSG_BIT_C:
                        {
                            destination_address[index2] = nmea[index1++];

                            break;
                        }

                        case MSG_BIT_N_S:
                        {
                            if (ns == 'N')
                            {
                                destination_address[index2] = nmea[index1++] + 32;
                            }
                            else
                            {
                                destination_address[index2] = nmea[index1++];
                            }

                            index1++; // Skip '.'

                            break;
                        }

                        case MSG_BIT_100:
                        {
                            if ((long_deg < 10) || ((long_deg >= 100) && (long_deg <= 179)))
                            {
                                destination_address[index2] = nmea[index1++] + 32;
                            }
                            else
                            {
                                destination_address[index2] = nmea[index1++];
                            }
                            break;
                        }

                        case MSG_BIT_E_W:
                        {
                            if (ns == 'W')
                            {
                                destination_address[index2] = nmea[index1++] + 32;
                            }
                            else
                            {
                                destination_address[index2] = nmea[index1++];
                            }

                            break;
                        }
                    }
                }

                // Get longitude
                // --------------

                // Longitude Degrees Encoding

                index1 = _GetNMEACommaPos(nmea, 5);

                //printf("nmea[%d]=%s\ntemp=%d\n", index1, &nmea[index1], long_deg);

                if (long_deg < 10) // 0�9 degrees: d+28 is in the range 118�127 decimal,

                {
                    mice[1] = long_deg + 118; // corresponding to the ASCII characters v to DEL.
                }
                else
                { // Important Note: The longitude offset is set to +100
                    // degrees when the longitude is in the range 0�9 degrees.
                    if (long_deg < 100) // 10�99 degrees: d+28 is in the range 38�127 decimal

                    {
                        mice[1] = long_deg + 28; // (corresponding to the ASCII characters & to DEL), and
                    }
                    else // the longitude offset is +0 degrees.
                    {
                        if (long_deg < 110) // 100�109 degrees: d+28 is in the range 108�117 decimal,'

                        {
                            mice[1] = long_deg + 8; // (corresponding to the ASCII characters l to   DEL), and
                        }
                        else
                        {
                            // the longitude offset is +100 degrees.
                            mice[1] = long_deg - 72; // 110�179 degrees: d+28 is in the range 38�127 decimal
                            // (corresponding to the ASCII characters & to DEL), and
                            // the longitude offset is +100 degrees
                        }
                    }
                }

                //printf("mice[1] = %c\n", mice[1]);

                // Longitude Minutes Encoding

                index1 += 3;

                temp = ((nmea[index1] - 48) * 10) + (nmea[index1 + 1] - 48);

                //printf("nmea[%d]=%s\nmin=%d\n", index1, &nmea[index1], temp);

                if (temp < 0)
                {
                    temp = 0;
                }

                if (temp < 10) // 0�9 minutes: m+28 is in the range 88�97 decimal,

                {
                    mice[2] = temp + 88; // (corresponding to the ASCII characters X to a).
                }
                else
                {
                    mice[2] = temp + 28; // 10�59 minutes: m+28 is in the range 38�87 decimal
                    // (corresponding to the ASCII characters & to W).
                }

                //printf("mice[2] = %c\n", mice[2]);

                index1 += 3; // Skip '.'

                // Longitude Hundredths of Minutes Encoding

                temp = ((nmea[index1] - 48) * 10) + (nmea[index1 + 1] - 48);

                //printf("nmea[%d]=%s\nsec=%d\n", index1, &nmea[index1], temp);

                if (temp < 0)
                {
                    temp = 0;
                }

                mice[3] = temp + 28; // The h+28 byte in the Information field contains the encoded
                // value of the longitude hundredths of minutes, in the range
                // 0�99 minutes. This byte takes a value in the range 28 decimal
                // (corresponding to 0 hundredths of a minute) through 127 decimal
                // (corresponding to 99 hundredths of a minute).

                // Get heading and speed
                // ---------------------

                index1 = _GetNMEACommaPos(nmea, 7); // Get speed
                index2 = _GetNMEACommaPos(nmea, 8);
                i = 0;
                for (temp = index1; (temp < index2) && (i <= NMEA_BUFFER_LENGTH); temp++)
                {
                    tempstr[i] = nmea[temp];
                    i++;
                }

                speed = (int) atof(tempstr);

                index1 = _GetNMEACommaPos(nmea, 8); // Get track
                index2 = _GetNMEACommaPos(nmea, 9);
                i = 0;

                for (temp = index1; (temp < index2) && (i <= NMEA_BUFFER_LENGTH); temp++)
                {
                    tempstr[i] = nmea[temp];
                    i++;
                }

                track = (int) atof(tempstr);

                index1 = 48;

                if (track == 0)
                {
                    track = 360;
                }

                mice[4] = (speed < 200) ? 108 + (speed / 10) : 48 + ((speed - 200) / 10);

                a = speed - ((speed / 10) * 10);

                mice[5] = 32 + (10 * a) + (track / 100);

                b = track - ((track / 100) * 100);

                mice[6] = 28 + b;

                mice[7] = 'g'; // Small Aircraft
                mice[8] = '/';

                switch (*nmea_status)
                // Update status

                {
                    case VALID:
                    {
                        break;
                    }

                    case ONLY_GPGGA:
                    {
                        *nmea_status = VALID;

                        break;
                    }

                    default:
                    {
                        *nmea_status = ONLY_GPRMC;

                        break;
                    }
                }

                //printf("done\n");

                return false;
            }
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
/**
 This function adds checksum to NMEA string
 */
void NMEA__AddChecksum(uint8_t *str)
{
    uint8_t checksum;
    uint8_t i, j;

    checksum = 0;

    i = strlen((char*) str);

    if (i > NMEA_BUFFER_LENGTH)
    {
        i = NMEA_BUFFER_LENGTH;
    }

    for (j = 1; j < i; j++)
    {
        checksum ^= str[j];
    }

    str[i] = '*';
    str[i + 1] = hex[((checksum & 0xF0) >> 4)];
    str[i + 2] = hex[(checksum & 0x0F)];
    str[i + 3] = 0;

}

//-------------------------------------------------------------------------------------------------
/**
 This function return the nmea status
 */
T_NMEA_STATUS NMEA__GetStatus(void)
{
    return nmea_status;
}

//-------------------------------------------------------------------------------------------------
/**
 This function prints the nmea status
 */
void NMEA__PrintStatus(void)
{
    printf("NMEA Status =");

    switch (nmea_status)
    {
        case INVALID:
        {
            printf("INVALID\n");
            break;
        }

        case VALID:
        {
            printf("VALID\n");
            break;
        }

        case ONLY_GPGGA:
        {
            printf("ONLY_GPGGA\n");
            break;
        }

        case ONLY_GPRMC:
        {
            printf("ONLY_GPRMC\n");
            break;
        }

        case ONLY_PCAID:
        {
            printf("ONLY_PCAID\n");
            break;
        }

        default:
            printf("ERROR\n");
    }
}

//-------------------------------------------------------------------------------------------------
/**
 This function resets the NMEA buffer
 */
void NMEA__Reset(void)
{
    NMEA__AddCharToBuffer(0);
}

//-------------------------------------------------------------------------------------------------
/**
 This function adds a char to the NMEA buffer
 */
void NMEA__AddCharToBuffer(uint8_t c)
{
    static uint8_t pnmea;

    switch (c)
    {
        case 0: // Reset
        {
            nmea_status = INVALID;
            pnmea = 0;

            break;
        }

        case '$':
        case '!':
        {
            pnmea = 0;

            break;
        }

        case 0x0A:
        {
            nmea_buffer[pnmea] = 0x00; // End of string

            pnmea = 0;

            if (_isNMEAStringValid(nmea_buffer))
            {
                NMEA__NMEAToMICE(nmea_buffer, mice_buffer, &nmea_status, destination_address);
            }

            break;
        }
    }

    if (c != 0x0A)
    {
        nmea_buffer[pnmea] = c;

        if (++pnmea > NMEA_BUFFER_LENGTH)
        {
            pnmea = 0;
        }
    }
}

//-------------------------------------------------------------------------------------------------
/**
 This function is not used
 */
void NMEA__GetDestinationAddress(uint8_t *adestination_address)
{
    strncpy((char*) adestination_address, (char*) destination_address, 7);
    adestination_address[7] = 0x00;
}

//-------------------------------------------------------------------------------------------------
/**
 This function returns a pointer to the MICE buffer
 */
void NMEA__GetMICEString(uint8_t *mice)
{
    int i;

    strncpy((char*) mice, (char*) mice_buffer, 13);
    mice[13] = 0x00;
}

//-------------------------------------------------------------------------------------------------
/**
 This function inserts a NMEA string for debug
 */
void NMEA__Test(void)
{
    int i;

    static uint8_t temp_nmea_buffer[NMEA_BUFFER_LENGTH];

    NMEA__AddCharToBuffer(0);

    strncpy((char*) temp_nmea_buffer, "$GPRMC,100931,A,5832.472,N,01620.769,E,000.0,000.0,130708,002.6,E*75", 68);

    temp_nmea_buffer[68] = 0x0A;
    temp_nmea_buffer[69] = 0x00;

    i = 0;

    while (temp_nmea_buffer[i] != 0x00)
    {
        NMEA__AddCharToBuffer(temp_nmea_buffer[i++]);
    }

    strncpy((char*) temp_nmea_buffer, "$GPGGA,100932,5832.472,N,01620.768,E,1,03,19,-00003,M,0030,M,,*43", 66);

    temp_nmea_buffer[65] = 0x0A;
    temp_nmea_buffer[66] = 0x00;

    i = 0;

    while (temp_nmea_buffer[i] != 0x00)
    {
        NMEA__AddCharToBuffer(temp_nmea_buffer[i++]);
    }
}

