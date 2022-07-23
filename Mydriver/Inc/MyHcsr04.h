/*
 * MyHcsr04.h
 *
 *  Created on: Jun 26, 2022
 *      Author: Lenovo
 */

#ifndef INC_MYHCSR04_H_
#define INC_MYHCSR04_H_

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "tim.h"

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t pinNumber;
	TIM_HandleTypeDef* htim;
	uint32_t distance;
	uint32_t tick;
}MyHcsr04_t;

extern MyHcsr04_t hcsr04;

void MyHcsr04_Init();
void MyHcsr04_Loop();


#endif /* INC_MYHCSR04_H_ */
