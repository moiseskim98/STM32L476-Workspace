#include "stm32l4xx.h"
#include "systick.h"




#define SYSTICK_LOAD_VALUE 			4000
#define	CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<2)
#define	CTRL_COUNTFLAG				(1U<<16)
#define CTRL_TICKINT				(1U<<1)

#define ONE_SEC_LOAD				4000000


void systickDelayMs(int delay)
{
	// Recargar con el numero de ciclos por milisegundo
	SysTick->LOAD = SYSTICK_LOAD_VALUE;

	// Limpiar el valor actual del registro de systick
	SysTick->VAL = 0;

	// Habilitar systick y seleccionar un reloj interno.
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for (int i=0; i<delay; i++)
	{
		// esperar hasta que la bandera esté encendida
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}

	}

	SysTick->CTRL = 0;

}

void systick_1hz_interrupt(void)
{
	// Recargar con el numero de ciclos por milisegundo
		SysTick->LOAD = ONE_SEC_LOAD;

		// Limpiar el valor actual del registro de systick
		SysTick->VAL = 0;

		// Habilitar systick y seleccionar un reloj interno y las interrupciones.
		 SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC | CTRL_TICKINT;

}
