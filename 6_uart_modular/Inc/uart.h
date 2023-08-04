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

void uart2_tx_init(void);
void uart2_write(int message);

#endif /* UART_H_ */
