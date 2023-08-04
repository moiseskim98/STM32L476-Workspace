/*
 * adc.h
 *
 *  Created on: Apr 10, 2023
 *      Author: hkim
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void pc0_adc1_init(void);
void start_adc1_conversion(void);
uint32_t adc1_read(void);

#endif /* ADC_H_ */
