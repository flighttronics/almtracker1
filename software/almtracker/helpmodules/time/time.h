/******************************************************************************

 time.h for Allmogetracker

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
#ifndef _TIME_H_
#define _TIME_H_

#include <avr/pgmspace.h>

// Time/Date definition

struct TimeStruct
{
	uint8_t hour; // hour 0-23
	uint8_t min; // min 0-59
	uint8_t sec; // Sec 0-59
	uint16_t year; // Year ex 2008
	uint8_t month; // Month 1-12
	uint8_t date; // Date 1-31
	uint32_t nttsp; // Next Time To Send Packet
};

uint8_t TimeToSendNextPacket(void);

extern void InitTime(void);
extern void SetTime(uint8_t hour, uint8_t min, uint8_t sec);
extern void GetTime(uint8_t *hour, uint8_t *min, uint8_t *sec);
extern void SetNextTimeToSendPacket(uint16_t addsec);
extern void SetDate(uint16_t year, uint8_t month, uint8_t date);
extern void TicSec(void);

#endif /* _LED_H_ */
