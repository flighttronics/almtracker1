/******************************************************************************

 time.c for Allmogetracker

 Allmogetracker receives data from a GPS receiver and transmits it
 over amature radio using the APRS protocol version 1.0.

 Copyright (C)2008, Andreas Kingbäck  (andki234@gmail.com)
 
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

#include "globals.h"
#include "stddefines.h"

void InitTime(void)
{
	GPSTime.hour = 0;
	GPSTime.min = 0;
	GPSTime.sec = 0;
	GPSTime.year = 2008;
	GPSTime.month = 8;
	GPSTime.date = 1;
	GPSTime.nttsp = 0; // Next time to send packet i sec.
}

void SetTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	GPSTime.hour = hour;
	GPSTime.min = min;
	GPSTime.sec = sec;
}

void GetTime(uint8_t *hour, uint8_t *min, uint8_t *sec)
{
	*hour = GPSTime.hour;
	*min = GPSTime.min;
	*sec = GPSTime.sec;
}

void SetNextTimeToSendPacket(uint16_t addsec)
{
	GPSTime.nttsp = (uint32_t)(GPSTime.hour * 3600)
	    + (uint32_t)(GPSTime.min * 60) + (uint32_t) GPSTime.sec
	    + (uint32_t) addsec;
}

uint8_t TimeToSendNextPacket(void)
{
	if ((uint32_t)((GPSTime.hour * 3600) + (GPSTime.min * 60) + GPSTime.sec)
	    >= GPSTime.nttsp)
		return TRUE;
	else
		return FALSE;
}

void SetDate(uint16_t year, uint8_t month, uint8_t date)
{
	GPSTime.year = year;
	GPSTime.month = month;
	GPSTime.date = date;
}

