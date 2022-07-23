/*
 * MyAdc.h
 *
 *  Created on: Jun 25, 2022
 *      Author: Lenovo
 */

#ifndef INC_MYADC_H_
#define INC_MYADC_H_

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"

typedef struct {
	uint32_t adc_val;
	float temperature;
	uint32_t tick;
} MyAdc_t;

void MyAdcLoop(void);
void MyAdcInit(void);

extern MyAdc_t MyAdc;

#endif /* INC_MYADC_H_ */
