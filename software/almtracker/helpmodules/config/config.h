/******************************************************************************

 config.h for Allmogetracker

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

#ifndef _CONFIG_H_
#define _CONFIG_H_ 1

//-------------------------------------------------------------------------------------------------
//--------  I N C L U D E  ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include "eeprom.h"

//-------------------------------------------------------------------------------------------------
//--------  C O N S T A N T S  --------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static const char ready_P[] PROGMEM = {'R','e','a','d','y','!',0x0A,0x0D,0x00};

//-------------------------------------------------------------------------------------------------
//--------  E N U M S  ----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum
{
    VERBEACON, OPBEACON
} T_BEACON;


#define BUAD_19200 0x2F
#define BUAD_4800 0xBF

//-------------------------------------------------------------------------------------------------
//--------  F U N C T I O N S  --------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void CONFIG__CheckEEPROM(void);
void CONFIG__WriteDefaultSettings(void);
void EEPROM__ReadBlock(void* eeprom);
void EEPROM__WriteBlock(void* eeprom);
void CONFIG__IncRebootCounter(void);

void CONFIG__GetCallsign(char *callsign);
bool CONFIG__ValidCallsign(void);
void CONFIG__GetBeacon(char *beacon, char beacontype);

uint8_t CONFIG__GetBaud(void);
uint8_t CONFIG__GetTXRate(void);
int8_t  CONFIG__GetPath(void);

#endif /* _CONFIG_H_ */
