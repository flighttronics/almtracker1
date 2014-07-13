/******************************************************************************

 config.c for Allmogetracker

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
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

// General purpose include files
#include "StdDefines.h"

// App required include files.
#include "config.h"
#include "ax25.h"
#include "usart.h"
#include "version.h"

const struct TEEPROM config_eemem_data EEMEM;

struct TEEPROM e;

/******************************************************************************/
void CONFIG__CheckEEPROM(void)
/******************************************************************************/
{
    eeprom_read_block(&e, 0, EEPROM_DATA_SIZE);

    if (0 != strcmp((char*) e.eeok, "eeOK"))
    {
        CONFIG__WriteDefaultSettings();
    }
}
/******************************************************************************/
void CONFIG__WriteDefaultSettings(void)
/******************************************************************************/
{
    const char nocall[7] =
    {0x9C, 0x9E, 0x86, 0x82, 0x98, 0x98, 0x60};

    e.reboot_counter = 0;
    memcpy((char*) e.callsign_ee, nocall, sizeof(nocall)); // NOCALL
    e.send_packet_every_n_sec = 120; // 120sec
    e.path = 0; // Standard path
	e.baud = 0xBF; // 0xBF = 4800 Baud or 0x2F = 19200 BAUD 
    strcpy((char*) e.opbeacon_ee, "NOBEACON\0"); // NOBEACON
    strcpy((char*) e.eeok, "eeOK\0"); // Indicates that EEPROM is Aligned

    eeprom_write_block(&e, 0, EEPROM_DATA_SIZE);
}

/******************************************************************************/
void EEPROM__ReadBlock(void* eeprom)
/******************************************************************************/
{
    eeprom_read_block(eeprom, 0, EEPROM_DATA_SIZE);
}

/******************************************************************************/
void EEPROM__WriteBlock(void* eeprom)
/******************************************************************************/
{
    eeprom_write_block(eeprom, 0, EEPROM_DATA_SIZE);
}

/******************************************************************************/
void CONFIG__IncRebootCounter(void)
/******************************************************************************/
{
    eeprom_read_block(&e, 0, EEPROM_DATA_SIZE);

    e.reboot_counter++;

    eeprom_write_block(&e, 0, EEPROM_DATA_SIZE);
}

//******************************************************************************/
int8_t CONFIG__GetPath(void)
/*******************************************************************************/
{
    uint8_t result;

    eeprom_read_block(&e, 0, EEPROM_DATA_SIZE);

    memcpy(&result, &e.path, sizeof(result));

    return result;
}

//******************************************************************************/
extern uint8_t CONFIG__GetTXRate(void)
/*******************************************************************************/
{
    uint8_t result;

    eeprom_read_block(&e, 0, EEPROM_DATA_SIZE);

    memcpy(&result, &e.send_packet_every_n_sec, sizeof(result));

    if (result < 60)
    {
        result = 60;
    }

    return result;
}

//******************************************************************************/
extern uint8_t CONFIG__GetBaud(void)
/*******************************************************************************/
{
	uint8_t result;

	eeprom_read_block(&e, 0, EEPROM_DATA_SIZE);

	memcpy(&result, &e.baud, sizeof(result));

	return result;
}

/******************************************************************************/
extern void CONFIG__GetCallsign(char *callsign)
/*******************************************************************************
 * ABSTRACT:
 *
 *
 * INPUT:		address	Starting address for the string
 * OUTPUT:	None
 * RETURN:	CALLSIGN
 */
{
    eeprom_read_block(&e, 0, EEPROM_DATA_SIZE);

    memcpy(callsign, e.callsign_ee, 7);
}

/******************************************************************************/
extern bool CONFIG__ValidCallsign(void)
/*******************************************************************************
 * ABSTRACT: Checks if configured callsign is NOCALL
 *
 * INPUT:   None
 * OUTPUT:  None
 * RETURN:  true if callsign equals NOCALL
 */
{
    char callsign[7];
    char nocall[6] =
    {0x9C, 0x9E, 0x86, 0x82, 0x98, 0x98}; // NOCALL

    CONFIG__GetCallsign(callsign);

    if (0 == memcmp(nocall, callsign, 6))
    {
        return false;
    }
    else
    {
        return true;
    }
}

/******************************************************************************/
extern void CONFIG__GetBeacon(char *beacon, char beacontype)
/*******************************************************************************
 * ABSTRACT:
 *
 * INPUT:		address	Starting address for the string
 * OUTPUT:	None
 * RETURN:
 */
{
    switch (beacontype)
    {
        case VERBEACON:
        {
            strcpy_P(beacon, firm_version_P);
            break;
        }

        case OPBEACON:
        {
            eeprom_read_block(&e, 0, EEPROM_DATA_SIZE);
            memcpy(beacon, e.opbeacon_ee, 50);
            break;
        }
    }

    return;
}

