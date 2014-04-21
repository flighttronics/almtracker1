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

#ifndef _ax25_H_
#define _ax25_H_

/****************************************************************************
 ------------------ GLOBAL (Variables, Constants & Defines) ----------------
 ****************************************************************************/

#define	MARK (168)  			// 167 - works from 190 to 155 (1200 Hz.)
#define	SPACE (209) 			// 213 - works from 204 to 216 (2200 Hz.)

unsigned char Txtone; // Used in main.c SIGNAL(SIG_OVERFLOW2)
unsigned char DestAddress[7]; // Used to set the packet destination adress
unsigned char MICEStr[15]; // Used to store the MIC-E format string

/****************************************************************************
 ---------------------- GLOBAL (Function Prototypes)  ----------------------
****************************************************************************/

extern void AX25__SendHeader(void);
extern void AX25__SendFooter(void);
extern void AX25__SendPath(void);
extern void AX25__SendCallsign(void);
extern void AX25__SendDestination(void);
extern void AX25__SendByte(unsigned char inbyte);
extern void AX25__CrcBit(int txbyte);
extern void AX25__SendASCIIebyte(unsigned short value);
extern void AX25__SendString(char *szString);
extern void AX25__SendEEPROMString(unsigned int address);

#endif /* _ax25_H_ */

