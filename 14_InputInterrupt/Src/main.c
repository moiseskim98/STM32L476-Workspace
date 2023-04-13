#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define PIN5				(1U<<5)
#define PIN13				(1U<<13)

#define LED_PIN				PIN5	// PA5
#define PUSH_BUTTON			PIN13	// PC13

#define AHB2ENR_GPIOAEN		(1U<<0)
#define AHB2ENR_GPIOCEN		(1U<<2)

#define MODER_MODE5_0		(1U<<10)
#define MODER_MODE5_1		(1U<<11)
#define MODER_MODE13_0		(1U<<26)
#define MODER_MODE13_1		(1U<<27)

int timestamp = 0;


// Conectar un jumper entre PA5 y PA6
int main(void)
{

	//-------PA5 configuration----------
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN;
	GPIOA->MODER |= MODER_MODE5_0;
	GPIOA->MODER &= ~MODER_MODE5_1;
	//----------------------------------

	//--------USART2--------------------
	uart2_rxtx_init();


	//---------iNTERRUPCION-------------
	pc13_exti_init();


	while(1)
	{

	}

	return 1;
}

static void exti_callback(void)
{
	printf("el boton fue presionado\n\r");
	GPIOA->ODR ^= LED_PIN;
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR1 & LINE13) != 0)
	{
		// Limpiar la bandera
		EXTI->PR1 |=  LINE13;
		// Hacer algo
		exti_callback();
	}
}
