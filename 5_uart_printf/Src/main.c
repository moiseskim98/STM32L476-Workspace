#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx.h"

#define PIN5				(1U<<5)
#define PIN13				(1U<<13)

#define LED_PIN				PIN5	// PA5
#define PUSH_BUTTON			PIN13	// PC13

#define AHB2ENR_GPIOAEN		(1U<<0)
#define AHB2ENR_GPIOCEN		(1U<<2)

#define APB1ENR1_USART2EN	(1U<<17)

#define MODER_MODE2_0		(1U<<4)
#define MODER_MODE2_1		(1U<<5)
#define MODER_MODE3_0		(1U<<6)
#define MODER_MODE3_1		(1U<<7)
#define MODER_MODE5_0		(1U<<10)
#define MODER_MODE5_1		(1U<<11)
#define MODER_MODE13_0		(1U<<26)
#define MODER_MODE13_1		(1U<<27)

#define CR1_M0				(1U<<12)
#define CR1_TE				(1U<<3)
#define CR1_UE				(1U<<0)

#define CR2_STOP_0			(1U<<12)
#define CR2_STOP_1			(1U<<13)

#define ISR_TXE				(1U<<7)

#define AFRL_AFSEL2_0		(1U<<8)
#define AFRL_AFSEL2_1		(1U<<9)
#define AFRL_AFSEL2_2		(1U<<10)
#define AFRL_AFSEL2_3		(1U<<11)
#define AFRL_AFSEL3_0		(1U<<8)
#define AFRL_AFSEL3_1		(1U<<9)
#define AFRL_AFSEL3_2		(1U<<10)
#define AFRL_AFSEL3_3		(1U<<11)

#define SYS_FREQ			4000000
#define APB1_CLK			SYS_FREQ
#define UART_BAUDRATE		115200


void uart2_tx_init(void);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_div(uint32_t PerphClk, uint32_t BaudRate);
void uart2_write(int message);

uint32_t hola;

int __io_putchar(int message)
{
	uart2_write(message);
	return message;
}

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

	//--------USART2--------------------
	uart2_tx_init();


	while(1)
	{
		hola = GPIOC->IDR;
		if(hola & PUSH_BUTTON)
		{
			GPIOA->ODR |= LED_PIN;
		}
		else
		{
			GPIOA->ODR &= ~LED_PIN;
			printf("hello from the other side!!!!\n\r");
		}
	}

	return 1;
}

void uart2_write(int message)
{
	// revisar si el registro de data este vacío
	while(!(USART2->ISR & ISR_TXE)){}

	// escribirle al registro de data de transmisión
	USART2->TDR = (message&0xFF);
}

void uart2_tx_init(void)
{
	// Configuración de pines TX:PA2 y RX:PA3
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN; // clk bus
	GPIOA->MODER |= (MODER_MODE2_1|MODER_MODE3_1);	// moder: 10
	GPIOA->MODER &= ~(MODER_MODE2_0|MODER_MODE3_0); // es el modo alternativo
	GPIOA->AFR[0] |= (AFRL_AFSEL2_0|AFRL_AFSEL2_1|AFRL_AFSEL2_2); // AF7
	GPIOA->AFR[0] |= (AFRL_AFSEL3_0|AFRL_AFSEL3_1|AFRL_AFSEL3_2); // AF7
	GPIOA->AFR[0] &= ~(AFRL_AFSEL2_3|AFRL_AFSEL3_3); // AF7

	// Configuración del clk para modulo uart
	RCC->APB1ENR1 |= APB1ENR1_USART2EN;

	// Configurar el BaudRate
	uart_set_baudrate(USART2, SYS_FREQ, UART_BAUDRATE);

	// Configurar largo de palabra: 8 bits
	USART2->CR1 |= CR1_M0; // word length

	// Configuración paro del tx: 1 bit
	USART2->CR2 &= ~(CR2_STOP_0|CR2_STOP_1); // 1 bit

	// Habilitar TX
	USART2->CR1 = CR1_TE; // habilitar tx

	// Habilitar UART
	USART2->CR1 |= CR1_UE;
}


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_div(PeriphClk,BaudRate);
}

static uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}


