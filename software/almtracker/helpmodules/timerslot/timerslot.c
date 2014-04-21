/*
 * timerslot.c
 *
 *  Created on: 10 sep 2010
 *      Author: SW Development
 */

#ifndef TIMERSLOT_C_
#define TIMERSLOT_C_

#include "led.h"
#include "main.h"

#include <avr/interrupt.h>
#include <avr/io.h>

/******************************************************************************/
void TIMERSLOT__1s(void)
/*******************************************************************************
 * ABSTRACT:    This routine is called every 1s and decreses the time to send
 *              the next packet variable.
 *
 * INPUT:   None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    DecrementTimeToTransmit();
}

/******************************************************************************/
void TIMERSLOT__10ms(void)
/*******************************************************************************
 * ABSTRACT:    This routine is called every 100ms and decreses the on time for
 *              the status LED
 *
 * INPUT:   None
 * OUTPUT:  None
 * RETURN:  None
 */
{
    LED__DecrementLEDOnTime();
}

/******************************************************************************/
SIGNAL(TIMER1_OVF_vect)
/*******************************************************************************
 * ABSTRACT:    This routine is called every 10ms
 *
 * INPUT:  None
 * OUTPUT: None
 * RETURN: None
 */
{
    static uint_least16_t tic_count = 0;

    TIMERSLOT__10ms();

    if (((++tic_count) % 100) == 0)
    {
        TIMERSLOT__1s();
    }

    TCNT1H = 0xFF;
    TCNT1L = 0x6F;
} // End SIGNAL(SIG_OVERFLOW1)

#endif /* TIMERSLOT_C_ */
