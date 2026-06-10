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
#include "joy.h"
#include "lpuart.h"


state_machine state;
static uint8_t error_printed = 0;


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
		state = ERROR;
	}

}


state_machine StateMachine_GetMode(void)
{
	return state;
}

void StateMachine_ChangeMode(state_machine new_state)
{
    state = new_state;
    if(new_state == UART || new_state == MANUAL)
    {
        error_printed = 0;
    }
}

void StateMachine_Run(void)
{
	//StateMachine_SetMode();

	if(state == IDLE)
	{
		// IDLE MODE
	}
	else if(state == UART)
	{
		UART_Protocol_Process();
	}
	else if(state == SCAN)
	{
		//SCAN MODE
	}
	else if(state == MANUAL)
	{
		Joy_Read();
	}
	else if(state == ERROR)
	{
	    if(error_printed == 0)
	    {
	        SendString("ERROR: UNKNOWN MODE\r\n");
	        TakeModeAGAIN();

	        error_printed = 1;
	    }
	}
}










