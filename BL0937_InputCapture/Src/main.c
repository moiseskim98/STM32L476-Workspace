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



#define PRESCALER_TIM3 		4
#define SYS_FREQ			4000000

unsigned int timestamp1 = 0;
unsigned int timestamp2 = 0;
int diferencia = 0;
float frecuencia = 100;
uint8_t Is_First_Captured = 0;

// Conectar un jumper entre PA5 y PA6
int main(void)
{

	uart2_rxtx_init();
	tim2_pa5_output_compare();
	tim3_pa6_input_capture(PRESCALER_TIM3);


	while(1)
	{
		// Esperar hasta que el flanco es capturado
		//while(!(TIM3->SR & SR_CC1IF)){}


		if(TIM3->SR & TIM_SR_CC1IF){

			if (Is_First_Captured == 0) // cuando no se ha capturado el primer flanco positivo
			{
				timestamp1 = TIM3->CCR1; // lee el primer valor
				Is_First_Captured = 1;  // set the first captured as true
			}
			else   // cuando ya se capturó la  primera vez, se prosigue a la segunda toma
			{
				timestamp2 = TIM3->CCR1;  // lee el segundo valor


				if (timestamp2 > timestamp1)
				{
					diferencia = timestamp2-timestamp1;
				}

				else if (timestamp1 > timestamp2) // si ocurrió overflow
				{
					diferencia = (0xffff - timestamp1) + timestamp2;
				}

				float refClock = SYS_FREQ/(PRESCALER_TIM3);

				frecuencia = refClock/diferencia;


				Is_First_Captured = 0; // set it back to false
				TIM3->CNT = 0;  // reinicia el contador

				printf("La frecuencia es: %d\n\r",(int)frecuencia);
			}
		}
	}

	return 1;
}
