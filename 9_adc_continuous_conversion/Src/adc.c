#include "adc.h"
#include "stm32l4xx.h"

#define AHB2ENR_ADCEN			(1U<<13)
#define AHB2ENR_GPIOCEN			(1U<<2)

#define CCIPR_ADCSEL_0			(1U<<28)
#define CCIPR_ADCSEL_1			(1U<<29)

#define GPIOx_MODER0_0			(1U<<0)
#define GPIOx_MODER0_1			(1U<<1)

#define ASCR_ASC0				(1U<<0)

#define SQR1_L_0				(1U<<0)
#define SQR1_L_1				(1U<<1)
#define SQR1_L_2				(1U<<2)
#define SQR1_L_3				(1U<<3)
#define SQR1_SQ1_0				(1U<<6)
#define SQR1_SQ1_1				(1U<<7)
#define SQR1_SQ1_2				(1U<<8)
#define SQR1_SQ1_3				(1U<<9)
#define SQR1_SQ1_4				(1U<<10)

#define IER_ADRDYIE				(1U<<0)

#define ISR_ADRDY				(1U<<0)
#define ISR_EOC					(1U<<2)

#define CR_ADEN					(1U<<0)
#define CR_ADSTART				(1U<<2)
#define CR_ADVREGEN				(1U<<28)
#define CR_DEEPPWD				(1U<<29)

#define CFGR_CONT				(1U<<13)



// CANAL 1
void pc0_adc1_init(void)
{
	// Configuracion del clk para GPIOC
	RCC->AHB2ENR |= AHB2ENR_GPIOCEN;

	// Configurar PC0 como analogo
	GPIOC->MODER |= (GPIOx_MODER0_1|GPIOx_MODER0_0);
	GPIOC->ASCR |= ASCR_ASC0;

	//******Configuracion del modulo de ADC******

	// Configuracion del reloj para el modulo
	RCC->AHB2ENR |= AHB2ENR_ADCEN;

	// Configurar el clk source del ADC
	RCC->CCIPR |= (CCIPR_ADCSEL_0|CCIPR_ADCSEL_1);

	// Salir del modo deep-power-down
	ADC1->CR &= ~(CR_DEEPPWD);
	//ADC1->CR &= 0x00000000;

	// Habilitar el regulador del adc
	ADC1->CR |= CR_ADVREGEN;

	// Vref enable
	// ADC123_COMMON->CCR |= (1U<<22);

	// Configuracion del inicio de la secuencia de conversion
	ADC1->SQR1 &= ~(SQR1_SQ1_1|SQR1_SQ1_2|SQR1_SQ1_3|SQR1_SQ1_4);
	ADC1->SQR1 |= SQR1_SQ1_0;

	// Configuración de la longitud de la secuencia de conversion
	ADC1->SQR1 &= ~(SQR1_L_0|SQR1_L_1|SQR1_L_2|SQR1_L_3);

	// Limpiar la bandera de conversión del adc
	ADC1->ISR |= ISR_ADRDY;

	// Inicializa el adc
	ADC1->CR |= CR_ADEN;

	// Esperar que inicializó
	while(!(ADC1->ISR & ISR_ADRDY)){}
	ADC1->ISR |= ISR_ADRDY;

}

void start_adc1_conversion(void)
{
	// Habilitar conversión continua
	ADC1->CFGR |= CFGR_CONT;

	// Comienza la conversión
	ADC1->CR |= CR_ADSTART;
}

uint32_t adc1_read(void)
{
	// Esperar a que termine la conversión
	while(!(ADC1->ISR & ISR_EOC)){}

	// Lee la conversion
	return (ADC1->DR);
}
