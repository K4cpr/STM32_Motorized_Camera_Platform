/*
 * motor.c
 *
 *  Created on: May 26, 2026
 *      Author: kapi-
 */

#include "motor.h"
#include "main.h"
#include <stdint.h>

void Motor_Init(void)
{
	//PB14 - PWM

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	GPIOD->MODER &= ~(GPIO_MODER_MODE9 | GPIO_MODER_MODE10);
	GPIOB->MODER &= ~(GPIO_MODER_MODE14);

	// Output (01)
	GPIOD->MODER |= (GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0);

	// Alternate 10
	GPIOB->MODER |= (GPIO_MODER_MODE14_1);
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL14_1 | GPIO_AFRH_AFSEL14_2 | GPIO_AFRH_AFSEL14_3);

	// Medium Speed (01)
	GPIOD->OSPEEDR |= (GPIO_OSPEEDR_OSPEED9_0 | GPIO_OSPEEDR_OSPEED10_0);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED14_0);
}

void Motor_Left(void)
{
	//PD10
	GPIOD->BSRR = GPIO_BSRR_BS10 | GPIO_BSRR_BR9;
}

void Motor_Right(void)
{
	//PD9
	GPIOD->BSRR = GPIO_BSRR_BR10 | GPIO_BSRR_BS9;
}

void Motor_Off(void)
{
	GPIOD->BSRR = GPIO_BSRR_BR10 | GPIO_BSRR_BR9;
}


uint16_t Speed(uint8_t speed)
{
	uint16_t Value_Speed = 0;

	Value_Speed = speed * 10;

	return Value_Speed;
}




