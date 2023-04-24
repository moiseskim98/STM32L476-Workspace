#include "stm32l4xx.h"


#define APB1ENR_TIM2EN			(1U<<0)

#define	CR1_CEN					(1U<<0)

#define DIER_UIE				(1U<<0)


void tim2_1hz_init(void)
{
	// Habilitar el acceso al reloj al timer 2
	RCC->APB1ENR1 |= APB1ENR_TIM2EN;

	// Setear el prescaler
	TIM2->PSC = 400 - 1;	// 4 000 000 / 400 = 10 000

	// Setear el valor de auto-recarga
	TIM2->ARR = 10000 - 1;	//	10 000/10 000 = 1

	// Limpiar el contador
	TIM2->CNT = 0;

	// Habilitar el timer
	TIM2->CR1 |= CR1_CEN;

}

void tim2_1hz_interrupt_init(void)
{
	// Habilitar el acceso al reloj al timer 2
	RCC->APB1ENR1 |= APB1ENR_TIM2EN;

	// Setear el prescaler
	TIM2->PSC = 400 - 1;	// 4 000 000 / 400 = 10 000

	// Setear el valor de auto-recarga
	TIM2->ARR = 10000 - 1;	//	10 000/10 000 = 1

	// Limpiar el contador
	TIM2->CNT = 0;

	// Habilitar el timer
	TIM2->CR1 |= CR1_CEN;

	/* Habilitar TIM interrupt */
	TIM2->DIER |= DIER_UIE;

	/* Habilitar TIM interrupt en NVIC */
	NVIC_EnableIRQ(TIM2_IRQn);

}
