/******************************************************************************

 eeprom.h for Allmogetracker

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

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <avr/eeprom.h>

struct TEEPROM
{
    uint32_t reboot_counter;            // Reboot counter
    uint8_t callsign_ee[7];             // Your CallSign + SSID
    uint8_t send_packet_every_n_sec;    // Transmit Rate in seconds
    uint8_t opbeacon_ee[50];            // Beacon text
	uint8_t path;						// Path
	uint8_t baud;						// Baud setting for device 
    uint8_t eeok[5];                    // eeOK string
};

#define EEPROM_DATA_SIZE sizeof(struct TEEPROM)

#endif
