/******************************************************************************
 tonegenerator.h for Allmogetracker

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

#ifndef TONEGENERATOR_H_
#define TONEGENERATOR_H_

uint8_t TONEGENERATOR__GetTransmitStatus(void);
uint8_t TONEGENERATOR__GetUSARTRXFlag(void);
void TONEGENERATOR__SetTransmitStatus(uint8_t tx);
void TONEGENERATOR__SetUSARTRXFlag(uint8_t flag);

#endif /* TONEGENERATOR_H_ */
