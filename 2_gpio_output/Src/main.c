#include "stm32l4xx.h"

#define PIN5			(1U<<5)
#define LED_PIN			PIN5

#define AHB2ENR_GPIOAEN	(1U<<0)
#define MODER_MODE5_0	(1U<<10)
#define MODER_MODE5_1	(1U<<11)


int main(void)
{
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN;

	GPIOA->MODER |= MODER_MODE5_0;
	GPIOA->MODER &= ~MODER_MODE5_1;

	while(1)
	{
		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++){}
	}

	return 1;
}

