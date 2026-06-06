/*
 * encoder.c
 *
 *  Created on: Jun 6, 2026
 *      Author: kapi-
 */


#include "main.h"
#include "encoder.h"
#include "tim.h"
#include "sys_clocks.h"

static int32_t position = 0;
static int32_t angle = 0;
static int32_t rpm = 0;
static uint32_t last_time = 0;
static uint16_t last_CNT;

void encoder_init(void)
{
	//PC6 - A
	//PC7 - B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	GPIOC->MODER &= ~(GPIO_MODER_MODE6);
	GPIOC->MODER &= ~(GPIO_MODER_MODE7);

	GPIOC->MODER |= GPIO_MODER_MODE6_1;
	GPIOC->MODER |= GPIO_MODER_MODE7_1;

	GPIOC->AFR[0] &= ~(GPIO_AFRL_AFSEL6);
	GPIOC->AFR[0] &= ~(GPIO_AFRL_AFSEL7);

	GPIOC->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOC->AFR[0] |= GPIO_AFRL_AFSEL7_1;

	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD6);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD7);

	Tim3Init();
	Tim3_Start();
}

uint16_t encoder_GetResult(void)
{
	return TIM3->CNT;
}

void encoder_reset(void)
{
	position = 0;
	angle = 0;
	rpm = 0;
	last_time = GetSystemTick();
	last_CNT = 0;
	TIM3->CNT = 0;
}

int16_t encoder_GetChange(void)
{
	uint16_t current_CNT = encoder_GetResult();
	int16_t result = current_CNT - last_CNT;
	last_CNT = current_CNT;
	return result;
}


int32_t encoder_GetPositionAngle(void)
{
	return angle;
}

int32_t encoder_GetSpeed(void)
{
	return rpm;
}


void encoder_Update(void)
{
    int16_t change = encoder_GetChange();

    position += change;
    angle = position * 360 / 7680;

    uint32_t current_time = GetSystemTick();

    if((current_time - last_time) >= 100)
    {
        rpm = change * 60000 / 7680;
        last_time = current_time;
    }


}



