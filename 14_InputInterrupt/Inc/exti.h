/*
 * exti.h
 *
 *  Created on: 13/04/2023
 *      Author: hkim
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32l4xx.h"
void pc13_exti_init(void);

#define	LINE13			(1U<<13)

#endif /* EXTI_H_ */
