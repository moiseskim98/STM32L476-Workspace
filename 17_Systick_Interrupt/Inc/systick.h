/*
 * systick.h
 *
 *  Created on: 12/04/2023
 *      Author: hkim
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

void systickDelayMs(int n);
void systick_1hz_interrupt(void);

#endif /* SYSTICK_H_ */
