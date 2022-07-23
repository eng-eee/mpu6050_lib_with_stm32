/*
 * MyHcsr04.c
 *
 *  Created on: Jun 26, 2022
 *      Author: Lenovo
 */


#include "MyHcsr04.h"

static uint8_t isFirstCapture;
static uint32_t first_capture;
static uint32_t second_capture;
static uint32_t diff_captures;
static uint32_t distance;

extern TIM_HandleTypeDef htim1;
MyHcsr04_t hcsr04 = { 0 };

static void My_usDelay(uint32_t time)
{
	__HAL_TIM_SET_COUNTER(hcsr04.htim,0);
	while(__HAL_TIM_GET_COUNTER(hcsr04.htim) < time);

}

void MyHcsr04_Init(void)
{
	hcsr04.distance = 0;
	hcsr04.GPIOx = GPIOA;
	hcsr04.pinNumber = GPIO_PIN_9;
	hcsr04.htim = &htim1;
	hcsr04.tick = 0;
	HAL_TIM_IC_Start_IT(hcsr04.htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void MyHcsr04_Loop()
{
	if(HAL_GetTick() - hcsr04.tick > 1000)
	{
		HAL_GPIO_WritePin(hcsr04.GPIOx, hcsr04.pinNumber, GPIO_PIN_SET);
		My_usDelay(10);
		HAL_GPIO_WritePin(hcsr04.GPIOx, hcsr04.pinNumber, GPIO_PIN_RESET);
		__HAL_TIM_ENABLE_IT(hcsr04.htim,TIM_IT_CC1);
		hcsr04.distance = distance;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,hcsr04.distance);
		hcsr04.tick = HAL_GetTick();
	}
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if(isFirstCapture == 0)
		{
			first_capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			isFirstCapture = 1;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else
		{
			second_capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			__HAL_TIM_SET_COUNTER(htim,0);
			diff_captures = second_capture - first_capture;

			distance = diff_captures * .034 /2;

			isFirstCapture = 0;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(htim,TIM_IT_CC1);
		}
	}
}

