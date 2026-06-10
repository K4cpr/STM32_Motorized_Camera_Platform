/*
 * uart_protocol.c
 *
 *  Created on: May 12, 2026
 *      Author: kapi-
 */


#include "main.h"
#include "lpuart.h"
#include <string.h>
#include <uart_protocol.h>
#include "state_machine.h"
#include <stdio.h>
#include "motor.h"
#include "oled.h"
#include "encoder.h"


command_t current_command;
uint8_t Value = 0;
uint8_t Speed_Percent = 50;
static char word[100];
static char word2[100];

void ParseWords(void)
{
	word[0] = '\0';
	word2[0] = '\0';

	uint8_t i = 0;
	uint8_t w = 0;
	uint8_t w2 = 0;

	uint8_t after_space = 0;
	while(BuffRx[i] != '\0')
	{
		if(BuffRx[i] == ' ')
		{
			after_space = 1;
			i++;
			continue;
		}
		if(after_space == 0)
		{
			if(w < 99)
			{
				word[w] = BuffRx[i];
				w++;
			}
		}
		else
		{
			if(w2 < 99)
			{
				word2[w2] = BuffRx[i];
				w2++;
			}
		}
		i++;

	}
	word[w] = '\0';
	word2[w2] = '\0';
}

uint8_t StringToInt(void)
{
	uint8_t num = 0;
	uint8_t result = 0;
	while(word2[num] != '\0')
	{
		if(word2[num] >= '0' && word2[num] <= '9')
		{
			result = result * 10 + (word2[num] - '0');
			if(result > 100)
			{
				StateMachine_ChangeMode(ERROR);
			}
		}
		num++;

	}
	return result;
}

void ParseCommand(void)
{
    ParseWords();

    if(strcmp(word, "UART") == 0)
    {
        current_command = CMD_UART;
    }
    else if(strcmp(word, "JOY") == 0)
    {
        current_command = CMD_JOY;
    }
    else if(state == IDLE || state == ERROR)
    {
        StateMachine_ChangeMode(ERROR);
        current_command = CMD_UNKNOWN;
    }
    else if(strcmp(word, "STATUS") == 0)
    {
        current_command = CMD_STATUS;
    }
    else if(strcmp(word, "LEFT") == 0)
    {
        current_command = CMD_LEFT;
    }
    else if(strcmp(word, "RIGHT") == 0)
    {
        current_command = CMD_RIGHT;
    }
    else if(strcmp(word, "STOP") == 0)
    {
        current_command = CMD_STOP;
    }
    else if(strcmp(word, "SPEED") == 0)
    {
        Value = StringToInt();

        if(state != UART)
        {
            current_command = CMD_UNKNOWN;
        }
        else if(motor_running == 0)
        {
            SendString("ERROR: MOTOR STOPPED\r\n");
            current_command = CMD_UNKNOWN;
        }
        else if(Value <= 100)
        {
            Speed_Percent = Value;
            current_command = CMD_SPEED;
        }
        else
        {
            SendString("ERROR: MAX SPEED = 100\r\n");
            current_command = CMD_UNKNOWN;
        }
    }
    else
    {
        current_command = CMD_UNKNOWN;
    }
}

void U_STATUS(void)
{
	motor_running = 0;
	SendString("SYSTEM OK"), SendString("\r\n");
	OLED_Clear();
	OLED_SetCursor(0,0);
	OLED_WriteString("SYSTEM OK");

	OLED_SetCursor(2,0);
	OLED_WriteString("SPEED: ");
	OLED_SetCursor(2,36);
	OLED_WriteInt(Speed_Percent);
	OLED_SetCursor(4,0);
	OLED_WriteString("MODE: UART");
}
void U_LEFT(void)
{
	motor_running = 1;

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
	OLED_WriteString("MODE: UART");

}
void U_RIGHT(void)
{
	motor_running = 1;
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
	OLED_WriteString("MODE: UART");
}
void U_STOP(void)
{
	motor_running = 0;

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
	OLED_WriteString("MODE: UART");
}
void U_SPEED(void)
{
	char tx[100];

	sprintf(tx, "SPEED MODE %d\r\n", Speed_Percent);

	SendString(tx);

	TIM15->CCR1 = Speed(Speed_Percent);
	encoder_ResetSpeed();
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
	OLED_WriteString("MODE: UART");


}
void U_UNKNOWN(void)
{
	motor_running = 0;
	SendString("UNKNOWN COMMAND"), SendString("\r\n");
	Motor_Off();
	OLED_Clear();
	OLED_SetCursor(2,0);
	OLED_WriteString("UNKNOWN COMMAND");
	OLED_SetCursor(4,0);
	OLED_WriteString("MODE: UART");
}


void HandleCommand(void)
{
	if(current_command == CMD_UART)
	{

	    StateMachine_ChangeMode(UART);

	    SendString("UART MODE\r\n");



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
		else if(last_move == 2)
		{

			OLED_SetCursor(0,0);
			OLED_WriteString("STOP");
		}

		OLED_SetCursor(2,0);
		OLED_WriteString("SPEED: ");
		OLED_SetCursor(2,36);
		OLED_WriteInt(Speed_Percent);
		OLED_SetCursor(4,0);
		OLED_WriteString("MODE: UART");
	}
	else if(current_command == CMD_JOY)
	{
	    StateMachine_ChangeMode(MANUAL);

	    SendString("JOY MODE\r\n");

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
		else if(last_move == 2)
		{

			OLED_SetCursor(0,0);
			OLED_WriteString("STOP");
		}

		OLED_SetCursor(2,0);
		OLED_WriteString("SPEED: ");
		OLED_SetCursor(2,36);
		OLED_WriteInt(Speed_Percent);
		OLED_SetCursor(4,0);
		OLED_WriteString("MODE: JOY");
	}

	else if(StateMachine_GetMode() == UART)
	{
	    if(current_command == CMD_STATUS)
	    {
	    	last_move = 2;
	        U_STATUS();
	    }
	    else if(current_command == CMD_LEFT)
	    {
	        last_move = 1;
	        U_LEFT();
	    }
	    else if(current_command == CMD_RIGHT)
	    {
	        last_move = 0;
	        U_RIGHT();
	    }
	    else if(current_command == CMD_STOP)
	    {
	    	last_move = 2;
	    	motor_running = 0;
	        U_STOP();
	    }
	    else if(current_command == CMD_SPEED)
	    {

	        U_SPEED();
	    }
	    else if(current_command == CMD_UNKNOWN)
	    {
	        U_UNKNOWN();
	    }
	}

}

void UART_Protocol_Process(void)
{
	if(LPUART_Available())
	{
		ParseCommand();
		HandleCommand();
		rx_ready = 0;
	}
}

void TakeMode(void)
{
    SendString("Hello take mode\r\n");
    SendString("MODE: UART\r\n");
    SendString("MODE: JOY\r\n");
}

void TakeModeAGAIN(void)
{

    SendString("MODE: UART\r\n");
    SendString("MODE: JOY\r\n");
}









