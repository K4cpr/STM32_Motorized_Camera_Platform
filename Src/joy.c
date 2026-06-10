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
#include "encoder.h"

//volatile int32_t encoder_angle;


char txt[20];
uint8_t last_move = 2;
uint8_t motor_running = 0;

static uint8_t button_currentRight = 0;
static uint8_t button_currentLeft  = 0;
static uint8_t button_currentUp    = 0;
static uint8_t button_currentDown  = 0;
static uint8_t button_currentOk    = 0;

static uint8_t button_stateRight = 0;
static uint8_t button_stateLeft  = 0;
static uint8_t button_stateUp    = 0;
static uint8_t button_stateDown  = 0;
static uint8_t button_stateOk    = 0;

static uint32_t last_button_tickRight = 0;
static uint32_t last_button_tickLeft  = 0;
static uint32_t last_button_tickUp    = 0;
static uint32_t last_button_tickDown  = 0;
static uint32_t last_button_tickOk    = 0;



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

uint8_t buttonRight_pressed(void)
{
    if(GPIOE->IDR & GPIO_IDR_ID0) return 0;
    return 1;
}

uint8_t buttonLeft_pressed(void)
{
    if(GPIOE->IDR & GPIO_IDR_ID1) return 0;
    return 1;
}

uint8_t buttonUp_pressed(void)
{
    if(GPIOE->IDR & GPIO_IDR_ID3) return 0;
    return 1;
}

uint8_t buttonDown_pressed(void)
{
    if(GPIOE->IDR & GPIO_IDR_ID2) return 0;
    return 1;
}

uint8_t buttonOk_pressed(void)
{
    if(GPIOE->IDR & GPIO_IDR_ID15) return 0;
    return 1;
}



void up(void)
{
	if(Speed_Percent <= 95 && motor_running == 1)
	{
		Speed_Percent +=5;
		encoder_ResetSpeed();
		TIM15->CCR1 = Speed(Speed_Percent);

		sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
		SendString(txt);
		OLED_Clear();

		if(last_move == 1)
		{

			OLED_SetCursor(0,0);
			OLED_WriteString("MOVING LEFT");
		}
		else if(last_move == 0)
		{

			OLED_SetCursor(0,0);
			OLED_WriteString("MOVING RIGHT");
		}


		OLED_SetCursor(2,0);
		OLED_WriteString("SPEED: ");
		OLED_SetCursor(2,36);
		OLED_WriteInt(Speed_Percent);

		OLED_SetCursor(4,0);
		OLED_WriteString("MODE: JOY");
	}
}

void down(void)
{
	if(Speed_Percent >= 5 && motor_running == 1)
	{
		Speed_Percent -=5;
		encoder_ResetSpeed();
		TIM15->CCR1 = Speed(Speed_Percent);
		sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
		SendString(txt);
		OLED_Clear();

		if(last_move == 1)
		{

			OLED_SetCursor(0,0);
			OLED_WriteString("MOVING LEFT");
		}
		else if(last_move == 0)
		{

			OLED_SetCursor(0,0);
			OLED_WriteString("MOVING RIGHT");
		}


		OLED_SetCursor(2,0);
		OLED_WriteString("SPEED: ");
		OLED_SetCursor(2,36);
		OLED_WriteInt(Speed_Percent);

		OLED_SetCursor(4,0);
		OLED_WriteString("MODE: JOY");
	}
}

void right(void)
{
	SendString("MOVE Right"), SendString("\r\n");
	sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
	SendString(txt);
    Motor_Right();
    encoder_ResetSpeed();

	OLED_Clear();
	OLED_SetCursor(0,0);
	OLED_WriteString("MOVING RIGHT");

	OLED_SetCursor(2,0);
	OLED_WriteString("SPEED: ");
	OLED_SetCursor(2,36);
	OLED_WriteInt(Speed_Percent);

	OLED_SetCursor(4,0);
	OLED_WriteString("MODE: JOY");
}

void left(void)
{
	SendString("MOVE LEFT"), SendString("\r\n");
	sprintf(txt, "SPEED=%d\r\n", Speed_Percent);
	SendString(txt);
    Motor_Left();
    encoder_ResetSpeed();

	OLED_Clear();
	OLED_SetCursor(0,0);
	OLED_WriteString("MOVING LEFT");

	OLED_SetCursor(2,0);
	OLED_WriteString("SPEED: ");
	OLED_SetCursor(2,36);
	OLED_WriteInt(Speed_Percent);

	OLED_SetCursor(4,0);
	OLED_WriteString("MODE: JOY");

}

void ok(void)
{
	SendString("STOP"), SendString("\r\n");
	SendString("SPEED 0"), SendString("\r\n");
	Motor_Off();
	encoder_ResetSpeed();

	OLED_Clear();
	OLED_SetCursor(0,0);
	OLED_WriteString("STOP");


	OLED_SetCursor(2,0);
	OLED_WriteString("SPEED: ");
	OLED_SetCursor(2,36);
	OLED_WriteInt(0);

	OLED_SetCursor(4,0);
	OLED_WriteString("MODE: JOY");
}


void Joy_Read(void)
{
    button_currentRight = buttonRight_pressed();

    if(button_currentRight == 1 && button_stateRight == 0 && (GetSystemTick() - last_button_tickRight) > 30)
    {
        last_move = 0;
        motor_running = 1;
        right();

        last_button_tickRight = GetSystemTick();
    }

    button_stateRight = button_currentRight;


    button_currentLeft = buttonLeft_pressed();

    if(button_currentLeft == 1 && button_stateLeft == 0 && (GetSystemTick() - last_button_tickLeft) > 30)
    {
        last_move = 1;
        motor_running = 1;
        left();

        last_button_tickLeft = GetSystemTick();
    }

    button_stateLeft = button_currentLeft;


    button_currentUp = buttonUp_pressed();

    if(button_currentUp == 1 && button_stateUp == 0 && (GetSystemTick() - last_button_tickUp) > 30)
    {
        up();

        last_button_tickUp = GetSystemTick();
    }

    button_stateUp = button_currentUp;


    button_currentDown = buttonDown_pressed();

    if(button_currentDown == 1 && button_stateDown == 0 && (GetSystemTick() - last_button_tickDown) > 30)
    {
        down();

        last_button_tickDown = GetSystemTick();
    }

    button_stateDown = button_currentDown;


    button_currentOk = buttonOk_pressed();

    if(button_currentOk == 1 && button_stateOk == 0 && (GetSystemTick() - last_button_tickOk) > 30)
    {
    	last_move = 2;
        motor_running = 0;
        ok();

        last_button_tickOk = GetSystemTick();
    }

    button_stateOk = button_currentOk;
}












