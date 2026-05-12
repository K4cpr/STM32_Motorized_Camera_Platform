/*
 * state_machine.c
 *
 *  Created on: May 12, 2026
 *      Author: kapi-
 */

#include "main.h"
#include <string.h>
#include "state_machine.h"
#include "uart_protocol.h"

state_machine state;


void StateMachine_Init(void)
{
	state = IDLE;

}

void StateMachine_SetMode(void)
{
	if(current_command == CMD_STATUS)
	{
		state = IDLE;
	}
	else if(current_command == CMD_LEFT)
	{
		state = UART;
	}
	else if(current_command == CMD_RIGHT)
	{
		state = UART;
	}
	else if(current_command == CMD_STOP)
	{
		state = IDLE;
	}
	else if(current_command == CMD_SPEED)
	{
		state = SCAN;
	}
	else if(current_command == CMD_UNKNOWN)
	{
		state = IDLE;
	}

}


state_machine StateMachine_GetMode(void)
{
	return state;
}

void StateMachine_Run(void)
{
	StateMachine_SetMode();

	if(state == IDLE)
	{
		//
	}
}










