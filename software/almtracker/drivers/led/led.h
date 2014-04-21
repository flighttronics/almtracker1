/******************************************************************************

 led.h for Allmogetracker

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

#ifndef _LED_H_
#define _LED_H_

/****************************************************************************
 ------------------ GLOBAL (Variables, Constants & Defines) ----------------
 ****************************************************************************/

#define TXLED    0x01
#define STATLED  0x02

/****************************************************************************
 ---------------------- GLOBAL (Function Prototypes)  ----------------------
 ****************************************************************************/

void LED__Init(void);
void LED__Set(int led, int state);
void LED__FlashStatusLED(void);
void LED__DecrementLEDOnTime(void);

#endif /* _LED_H_ */
