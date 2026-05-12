/*
 * uart_protocol.h
 *
 *  Created on: May 12, 2026
 *      Author: kapi-
 */

#ifndef UART_PROTOCOL_H_
#define UART_PROTOCOL_H_



typedef enum
{
    CMD_NONE,
    CMD_STATUS,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_STOP,
    CMD_SPEED,
    CMD_UNKNOWN

} command_t;

void ParseCommand(void);
void HandleCommand(void);
void UART_Protocol_Process(void);

#endif /* UART_PROTOCOL_H_ */
