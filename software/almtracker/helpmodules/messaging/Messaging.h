/******************************************************************************

 messaging.h for Allmogetracker

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

#ifndef _MESS_H_
#define _MESS_H_

// NMEA commands
enum ENMEA_COMMANDS
{
    GPRMC, GPGGA, PCAID
};

// NMEA Status types
enum ESTATUS
{
    INVALID, OnlyGPGGA, OnlyGPRMC, VALID, OnlyPCAID, SEND_EEPROM, WRITE_EEPROM
};

extern void MESSAGING__MsgInit(void);
extern char ValidFix(void);
extern unsigned char NMEAChecksum(void);
extern unsigned char NMEACheckForEqualTimeStamps(void);
extern unsigned char MsgPrepare_MICE(uint8_t Status);
extern unsigned char strpos(char *str, char c, uint8_t maxlen);
extern void MsgPrepare(void);
extern void MsgSendBeacon(void);
extern void MsgSendPos(void);
extern void MsgSendTelem(void);
extern void MsgSendAck(unsigned char *rxbytes, unsigned char msg_start);
extern void MsgHandler(unsigned char newchar);

#endif /* _MESS_H_ */
