#include "stm32l4xx.h"


#define APB1ENR_TIM2EN			(1U<<0)
#define APB1ENR_TIM3EN			(1U<<1)

#define	CR1_CEN					(1U<<0)

#define CCMR1_OC1M_0			(1U<<4)
#define CCMR1_OC1M_1			(1U<<5)
#define CCMR1_OC1M_2			(1U<<6)
#define CCMR1_OC1M_3			(1U<<16)
#define CCMR1_CC1S_0			(1U<<0)
#define CCMR1_CC1S_1			(1U<<1)
#define CCMR1_IC1F_0			(1U<<4)
#define CCMR1_IC1F_1			(1U<<5)
#define CCMR1_IC1F_2			(1U<<6)
#define CCMR1_IC1F_3			(1U<<7)



#define CCER_CC1E				(1U<<0)
#define CCER_CC1P				(1U<<1)
#define CCER_CC1NP				(1U<<3)


#define AHB2ENR_GPIOAEN			(1U<<0)

#define MODER_MODE5_0			(1U<<10)
#define MODER_MODE5_1			(1U<<11)
#define MODER_MODE6_0			(1U<<12)
#define MODER_MODE6_1			(1U<<13)

#define AFR5_TIM				(1U<<20)
#define AFR6_TIM				(1U<<25)


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


void tim2_pa5_output_compare(void)
{
	// Habilitar el acceso al reloj al GPIOA
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN;

	// Setear PA5 en modo función alternativa
	GPIOA->MODER |= MODER_MODE5_1;
	GPIOA->MODER &= ~MODER_MODE5_0;

	// Setear PA5 en la función alternativa como TIM2_CH1 AF01
	GPIOA->AFR[0] |= AFR5_TIM;

	// Habilitar el acceso al reloj al timer 2
	RCC->APB1ENR1 |= APB1ENR_TIM2EN;

	// Setear el prescaler
	TIM2->PSC = 400 - 1;	// 4 000 000 / 400 = 10 000

	// Setear el valor de auto-recarga
	TIM2->ARR = 10000 - 1;	//	10 000/10 000 = 1

	// Setear el modo output compare toggle mode
	TIM2->CCMR1 &= ~(CCMR1_OC1M_2|CCMR1_OC1M_3);
	TIM2->CCMR1 |= (CCMR1_OC1M_0|CCMR1_OC1M_1);

	// Habilitar tim2 ch1 en compare mode
	TIM2->CCER |= CCER_CC1E;


	// Limpiar el contador
	TIM2->CNT = 0;

	// Habilitar el timer
	TIM2->CR1 |= CR1_CEN;

}

void tim3_pa6_input_capture(unsigned int prescaler)
{
	// Habilitar reloj del GPIOA
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN;

	// Setear PA6 como modo de función alternativo
	GPIOA->MODER |= MODER_MODE6_1;
	GPIOA->MODER &= ~MODER_MODE6_0;

	// Setear PA6 función alternativa como TIM3_CH1 (AF01)
	GPIOA->AFR[0] |= AFR6_TIM;

	// Habilitar reloj al tim3
	RCC->APB1ENR1 |= APB1ENR_TIM3EN;

	// Setear el prescaler
	TIM3->PSC = prescaler -1; 	// 4 000 000 / 4 000 = 1 000

	// Setear CH1 como entrada
	TIM3->CCMR1 &= ~CCMR1_CC1S_1;
	TIM3->CCMR1 |= CCMR1_CC1S_0;

	// Configurar el filtro
	//TIM3->CCMR1 &= ~(CCMR1_IC1F_2|CCMR1_IC1F_3);
	//TIM3->CCMR1 |= (CCMR1_IC1F_1|CCMR1_IC1F_0);

	// Setear CH1 para capturar en cada flanco positivo
	TIM3->CCER &= ~(CCER_CC1P|CCER_CC1NP);

	// Habilitar el tim3 para el channel 1
	TIM3->CCER |= CCER_CC1E;

	// Habilitar el tim3
	TIM3->CR1 |= CR1_CEN;

	TIM3->SR = ~TIM_SR_CC1IF;

}
