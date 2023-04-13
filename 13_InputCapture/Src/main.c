#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "systick.h"
#include "tim.h"

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

	tim2_pa5_output_compare();
	tim3_pa6_input_capture();


	while(1)
	{

		// Esperar hasta que el flanco es capturado
		while(!(TIM3->SR & SR_CC1IF)){}

		// Leer el valor
		timestamp = TIM3->CCR1;
	}

	return 1;
}
