/*
 * uart.h
 *
 *  Created on: Mar 31, 2023
 *      Author: hkim
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "stm32l4xx.h"

#define ISR_RXNE			(1U<<5)


void uart2_rxtx_init(void);
void uart2_tx_init(void);
void uart2_write(int message);
uint8_t uart2_read(void);
void uart2_rx_interrupt_init(void);

#endif /* UART_H_ */
