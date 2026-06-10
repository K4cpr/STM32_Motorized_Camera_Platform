/*
 * uart_protocol.h
 *
 *  Created on: May 12, 2026
 *      Author: kapi-
 */

#ifndef UART_PROTOCOL_H_
#define UART_PROTOCOL_H_

extern char txt[20];
extern uint8_t last_move;
extern uint8_t motor_running;

typedef enum
{
    CMD_NONE,
    CMD_STATUS,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_STOP,
    CMD_SPEED,
    CMD_UNKNOWN,
	CMD_UART,
	CMD_JOY

} command_t;

void ParseWords(void);
uint8_t StringToInt(void);
void ParseCommand(void);

void U_STATUS(void);
void U_LEFT(void);
void U_RIGHT(void);
void U_STOP(void);
void U_SPEED(void);
void U_UNKNOWN(void);

void HandleCommand(void);
void UART_Protocol_Process(void);
void TakeMode(void);
void TakeModeAGAIN(void);


#endif /* UART_PROTOCOL_H_ */
