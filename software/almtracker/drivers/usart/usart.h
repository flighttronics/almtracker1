/******************************************************************************

 usart.h for Allmogetracker

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

#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>

/****************************************************************************
 ------------------ GLOBAL (Variables, Constants & Defines) ----------------
 ****************************************************************************/
#define	BUFFER_SIZE	0x20

/****************************************************************************
 ------------------ LOCAL (Variables, Constants & Defines) ----------------
 ****************************************************************************/
#ifdef _USART_C

static uint8_t in_buffer[BUFFER_SIZE];
static uint8_t in_head;
static uint8_t in_tail;

#endif

/****************************************************************************
 ---------------------- GLOBAL (Function Prototypes)  ----------------------
 ****************************************************************************/

void USART__Initialize(void);
void USART__Flush(void);
void USART__SendByte(uint8_t data);
void USART__SendHexByte(uint16_t hex);
void USART__SendString(const char *data);
void USART__SendString_P(const char *data);
void USART__Execute(void);

#endif /* _USART_H_ */
