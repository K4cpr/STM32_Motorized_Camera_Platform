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

command_t current_command;

void ParseCommand(void)
{
	static char word[100];
	uint8_t i = 0;
	while(BuffRx[i] != '\0')
	{
		word[i] = BuffRx[i];
		i++;
	}
	word[i] = '\0';

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
		current_command = CMD_SPEED;
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
		SendString("SPEED MODE"), SendString("\r\n");
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
		rx_ready = 0;
	}
}


