/*
 * joy.c
 *
 *  Created on: Jun 5, 2026
 *      Author: kapi-
 */


#include "main.h"
#include "stdio.h"
#include "joy.h"
#include "motor.h"
#include "tim.h"
#include "lpuart.h"
#include "oled.h"

uint8_t Speed_Percent2 = 50;
char txt[20];
uint8_t last_move = 2;
uint8_t motor_running = 0;

void joy_init(void)
{
	//PE15 - center
	//PE3 - UP
	//PE2 - DN
	//PE1 - LEFT
	//PE0 - RIGHT
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE15);
	GPIOE->MODER &= ~(GPIO_MODER_MODE0);
	GPIOE->MODER &= ~(GPIO_MODER_MODE1);
	GPIOE->MODER &= ~(GPIO_MODER_MODE2);
	GPIOE->MODER &= ~(GPIO_MODER_MODE3);

	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD15);
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD0);
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD1);
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD2);
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD3);

}



void up(void)
{
	if(Speed_Percent <= 95 && motor_running == 1)
	{
		Speed_Percent +=5;
		TIM15->CCR1 = Speed(Speed_Percent);

		sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
		SendString(txt);
		OLED_Clear();

		if(last_move == 1)
		{

			OLED_SetCursor(2,0);
			OLED_WriteString("MOVING LEFT");
		}
		else if(last_move == 0)
		{

			OLED_SetCursor(2,0);
			OLED_WriteString("MOVING RIGHT");
		}


		OLED_SetCursor(4,0);
		OLED_WriteString("SPEED: ");
		OLED_SetCursor(4,36);
		OLED_WriteInt(Speed_Percent);

		OLED_SetCursor(6,0);
		OLED_WriteString("MODE: JOY");
	}
}

void down(void)
{
	if(Speed_Percent >= 5 && motor_running == 1)
	{
		Speed_Percent -=5;
		TIM15->CCR1 = Speed(Speed_Percent);
		sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
		SendString(txt);
		OLED_Clear();

		if(last_move == 1)
		{

			OLED_SetCursor(2,0);
			OLED_WriteString("MOVING LEFT");
		}
		else if(last_move == 0)
		{

			OLED_SetCursor(2,0);
			OLED_WriteString("MOVING RIGHT");
		}


		OLED_SetCursor(4,0);
		OLED_WriteString("SPEED: ");
		OLED_SetCursor(4,36);
		OLED_WriteInt(Speed_Percent);

		OLED_SetCursor(6,0);
		OLED_WriteString("MODE: JOY");
	}
}

void right(void)
{
	SendString("MOVE Right"), SendString("\r\n");
	sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
	SendString(txt);
    Motor_Right();

	OLED_Clear();
	OLED_SetCursor(2,0);
	OLED_WriteString("MOVING RIGHT");

	OLED_SetCursor(4,0);
	OLED_WriteString("SPEED: ");
	OLED_SetCursor(4,36);
	OLED_WriteInt(Speed_Percent);

	OLED_SetCursor(6,0);
	OLED_WriteString("MODE: JOY");
}

void left(void)
{
	SendString("MOVE LEFT"), SendString("\r\n");
	sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
	SendString(txt);
    Motor_Left();

	OLED_Clear();
	OLED_SetCursor(2,0);
	OLED_WriteString("MOVING LEFT");

	OLED_SetCursor(4,0);
	OLED_WriteString("SPEED: ");
	OLED_SetCursor(4,36);
	OLED_WriteInt(Speed_Percent);

	OLED_SetCursor(6,0);
	OLED_WriteString("MODE: JOY");
}

void ok(void)
{
	SendString("STOP"), SendString("\r\n");
	SendString("SPEED 0"), SendString("\r\n");
	Motor_Off();

	OLED_Clear();
	OLED_SetCursor(2,0);
	OLED_WriteString("STOP");


	OLED_SetCursor(4,0);
	OLED_WriteString("SPEED: ");
	OLED_SetCursor(4,36);
	OLED_WriteInt(0);

	OLED_SetCursor(6,0);
	OLED_WriteString("MODE: JOY");
}





void Joy_Read(void)
{
    if(!(GPIOE->IDR & GPIO_IDR_ID0))
    {
    	last_move = 0;
    	motor_running = 1;

    	right();
        while(!(GPIOE->IDR & GPIO_IDR_ID0))
        {
        }
    }

    if(!(GPIOE->IDR & GPIO_IDR_ID1))
    {
    	last_move = 1;
    	motor_running = 1;

    	left();
        while(!(GPIOE->IDR & GPIO_IDR_ID1))
        {
        }
    }

    if(!(GPIOE->IDR & GPIO_IDR_ID3))
    {

    	up();
        while(!(GPIOE->IDR & GPIO_IDR_ID3))
        {
        }
    }

    if(!(GPIOE->IDR & GPIO_IDR_ID2))
    {

    	down();
        while(!(GPIOE->IDR & GPIO_IDR_ID2))
        {
        }
    }

    if(!(GPIOE->IDR & GPIO_IDR_ID15))
    {
    	motor_running = 0;
    	ok();

        while(!(GPIOE->IDR & GPIO_IDR_ID15))
        {
        }
    }

}












