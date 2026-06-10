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
#include "oled.h"

static int32_t position = 0;
static int32_t angle = 0;
static int32_t rpm = 0;
static uint32_t last_time = 0;
static uint16_t last_CNT;
static int32_t speed_counts = 0;
static uint8_t skip_speed_sample = 0;

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

void encoder_ResetSpeed(void)
{
    speed_counts = 0;
    rpm = 0;
    last_time = GetSystemTick();
    last_CNT = TIM3->CNT;
    skip_speed_sample = 1;
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

    speed_counts += change;

    uint32_t current_time = GetSystemTick();

    if((current_time - last_time) >= 500)
    {
        int32_t new_rpm = speed_counts * 12000 / 7680;

        if(new_rpm < 0)
        {
            new_rpm = -new_rpm;
        }

        if(skip_speed_sample)
        {
            skip_speed_sample = 0;
        }
        else
        {
            rpm = new_rpm;
        }

        speed_counts = 0;
        last_time = current_time;
    }
}

void OLED_ShowEncoderData(void)
{
    OLED_SetCursor(6,0);
    OLED_WriteString("ANGLE:");
    OLED_SetCursor(6,40);
    OLED_Write_Int(encoder_GetPositionAngle());

    OLED_SetCursor(7,0);
    OLED_WriteString("RPM:");
    OLED_SetCursor(7,40);
    OLED_Write_Int(encoder_GetSpeedint());
}

int32_t encoder_GetSpeedint(void)
{
    int32_t speed = rpm;

    if(speed < 0)
    {
        speed = -speed;
    }

    return speed;
}

