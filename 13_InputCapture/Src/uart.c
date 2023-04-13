#include "uart.h"

#define AHB2ENR_GPIOAEN		(1U<<0)

#define APB1ENR1_USART2EN	(1U<<17)

#define CR1_M0				(1U<<12)
#define CR1_TE				(1U<<3)
#define CR1_RE				(1U<<2)
#define CR1_UE				(1U<<0)

#define CR2_STOP_0			(1U<<12)
#define CR2_STOP_1			(1U<<13)

#define ISR_TXE				(1U<<7)
#define ISR_RXNE			(1U<<5)

#define USART2_AFSEL2_0		(1U<<8)
#define USART2_AFSEL2_1		(1U<<9)
#define USART2_AFSEL2_2		(1U<<10)
#define USART2_AFSEL2_3		(1U<<11)
#define USART2_AFSEL3_0		(1U<<12)
#define USART2_AFSEL3_1		(1U<<13)
#define USART2_AFSEL3_2		(1U<<14)
#define USART2_AFSEL3_3		(1U<<15)

#define MODER_MODE2_0		(1U<<4)
#define MODER_MODE2_1		(1U<<5)
#define MODER_MODE3_0		(1U<<6)
#define MODER_MODE3_1		(1U<<7)

#define SYS_FREQ			4000000
#define APB1_CLK			SYS_FREQ
#define UART_BAUDRATE		115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_div(uint32_t PerphClk, uint32_t BaudRate);


int __io_putchar(int message)
{
	uart2_write(message);
	return message;
}

void uart2_write(int message)
{
	// revisar si el registro de data de transmisión este vacío
	while(!(USART2->ISR & ISR_TXE)){}

	// escribirle al registro de data de transmisión
	USART2->TDR = (message&0xFF);
}

uint8_t uart2_read(void)
{
	// revisar si el registro de data de recibido este lleno
	while(!(USART2->ISR & ISR_RXNE)){}

	// regreso del dato
	return USART2->RDR;
}

void uart2_tx_init(void)
{
	// Configuración de pines TX:PA2 y RX:PA3
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN; // clk bus
	GPIOA->MODER |= (MODER_MODE2_1|MODER_MODE3_1);	// moder: 10
	GPIOA->MODER &= ~(MODER_MODE2_0|MODER_MODE3_0); // es el modo alternativo
	GPIOA->AFR[0] |= (USART2_AFSEL2_0|USART2_AFSEL2_1|USART2_AFSEL2_2); // AF7
	GPIOA->AFR[0] |= (USART2_AFSEL3_0|USART2_AFSEL3_1|USART2_AFSEL3_2); // AF7
	GPIOA->AFR[0] &= ~(USART2_AFSEL2_3|USART2_AFSEL3_3); // AF7

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

void uart2_rxtx_init(void)
{
	// Configuración de pines TX:PA2 y RX:PA3
	RCC->AHB2ENR |= AHB2ENR_GPIOAEN; // clk bus
	GPIOA->MODER |= (MODER_MODE2_1|MODER_MODE3_1);	// moder: 10
	GPIOA->MODER &= ~(MODER_MODE2_0|MODER_MODE3_0); // es el modo alternativo
	GPIOA->AFR[0] |= (USART2_AFSEL2_0|USART2_AFSEL2_1|USART2_AFSEL2_2); // AF7
	GPIOA->AFR[0] |= (USART2_AFSEL3_0|USART2_AFSEL3_1|USART2_AFSEL3_2); // AF7
	GPIOA->AFR[0] &= ~(USART2_AFSEL2_3|USART2_AFSEL3_3); // AF7

	// Configuración del clk para modulo uart
	RCC->APB1ENR1 |= APB1ENR1_USART2EN;

	// Configurar el BaudRate
	uart_set_baudrate(USART2, SYS_FREQ, UART_BAUDRATE);

	// Configurar largo de palabra: 8 bits
	USART2->CR1 |= CR1_M0; // word length

	// Configuración paro del tx: 1 bit
	USART2->CR2 &= ~(CR2_STOP_0|CR2_STOP_1); // 1 bit

	// Habilitar TX y RX
	USART2->CR1 = (CR1_TE|CR1_RE); // habilitar tx

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

