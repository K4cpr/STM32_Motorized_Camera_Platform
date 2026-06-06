/*
 * state_machine.h
 *
 *  Created on: May 12, 2026
 *      Author: kapi-
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "uart_protocol.h"

typedef enum
{
	IDLE,
	UART,
	MANUAL,
	SCAN,
	ERROR

}state_machine;

extern command_t current_command;

void StateMachine_Init(void);
void StateMachine_SetMode(void);
state_machine StateMachine_GetMode(void);
void StateMachine_ChangeMode(state_machine new_state);
void StateMachine_Run(void);


#endif /* STATE_MACHINE_H_ */
