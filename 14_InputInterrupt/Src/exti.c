/*
 * exti.c
 *
 *  Created on: 13/04/2023
 *      Author: hkim
 */


#include "exti.h"

#define AHB2ENR_GPIOCEN		(1U<<2)
#define APB2ENR_SYSCFGEN	(1U<<0)

#define MODER_MODE13_0		(1U<<26)
#define MODER_MODE13_1		(1U<<27)

#define EXTICR3_13_0		(1U<<4)
#define EXTICR3_13_1		(1U<<5)
#define EXTICR3_13_2		(1U<<6)
#define EXTICR3_13_3		(1U<<7)

#define IMR1_IM13			(1U<<13)

#define FTSR1_FT13			(1U<<13)


void pc13_exti_init(void)
{
	// Deshabilitar interrupciones globales (opcional)
	__disable_irq();

	// Habilitar acceso del reloj al GPIOC
	RCC->AHB2ENR |= AHB2ENR_GPIOCEN;

	// Setear PC13 como entrada
	GPIOC->MODER &= ~(MODER_MODE13_0|MODER_MODE13_1);

	// Habilitar acceso del reloj a SYSCFG
	RCC->APB2ENR |= APB2ENR_SYSCFGEN;

	// Seleccionar PORTC para EXTI13
	SYSCFG->EXTICR[3] &= ~(EXTICR3_13_0|EXTICR3_13_2|EXTICR3_13_3);
	SYSCFG->EXTICR[3] |= EXTICR3_13_1;

	// Desenmascarar EXTI13
	EXTI->IMR1 |= IMR1_IM13;

	// Seleccionar flanco positivo o negativo. (negativo)
	EXTI->FTSR1 |= FTSR1_FT13;

	// Habilitar la linea de EXTI13 en NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	// Habilitar interrupciones globales
	__enable_irq();

}
