#include "stm32l4xx.h"

#define PIN5			(1U<<5)
#define PIN13			(1U<<13)

#define LED_PIN			PIN5	// PA5
#define PUSH_BUTTON		PIN13	// PC13

#define AHB2ENR_GPIOAEN	(1U<<0)
#define AHB2ENR_GPIOCEN	(1U<<2)
#define MODER_MODE5_0	(1U<<10)
#define MODER_MODE5_1	(1U<<11)
#define MODER_MODE13_0	(1U<<26)
#define MODER_MODE13_1	(1U<<27)


int main(void)
{
	//-------PA5 configuration----------
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN;
	GPIOA->MODER |= MODER_MODE5_0;
	GPIOA->MODER &= ~MODER_MODE5_1;
	//----------------------------------

	//--------PC13 configuration--------
	RCC->AHB2ENR |= AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~(MODER_MODE13_0|MODER_MODE13_1);
	//----------------------------------

	while(1)
	{

		if(GPIOC->IDR & PUSH_BUTTON)
		{
			GPIOA->ODR |= LED_PIN;
		}
		else
		{
			GPIOA->ODR &= ~LED_PIN;
		}
	}

	return 1;
}

