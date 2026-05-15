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

command_t current_command;
state_machine state2;
uint8_t Value = 0;
uint8_t Speed_Percent = 0;
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
				state2 = ERROR;
			}
		}
		num++;

	}
	return result;
}

void ParseCommand(void)
{
	ParseWords();

	if(strcmp(word, "STATUS") == 0)
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
		if(Value <=100)
		{
			Speed_Percent = Value;
			current_command = CMD_SPEED;
		}
		else
		{
			SendString("error, max speed = 100");
			current_command = CMD_UNKNOWN;
		}
	}
	else
	{
		current_command = CMD_UNKNOWN;
	}

}

void HandleCommand(void)
{
	if(current_command == CMD_STATUS)
	{
		SendString("SYSTEM OK"), SendString("\r\n");
	}
	else if(current_command == CMD_LEFT)
	{
		SendString("MOVE LEFT"), SendString("\r\n");
	}
	else if(current_command == CMD_RIGHT)
	{
		SendString("MOVE RIGHT"), SendString("\r\n");
	}
	else if(current_command == CMD_STOP)
	{
		SendString("STOP"), SendString("\r\n");
	}
	else if(current_command == CMD_SPEED)
	{
		char tx[50];

		sprintf(tx, "SPEED MODE %d\r\n", Speed_Percent);

		SendString(tx), SendString("\r\n");
	}
	else if(current_command == CMD_UNKNOWN)
	{
		SendString("UNKNOWN COMMAND"), SendString("\r\n");
	}

}

void UART_Protocol_Process(void)
{
	if(LPUART_Available())
	{
		ParseCommand();
		HandleCommand();
		StateMachine_SetMode();
		rx_ready = 0;
	}
}


