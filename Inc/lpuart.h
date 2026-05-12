/*
 * lpuart.h
 *
 *  Created on: Feb 23, 2026
 *      Author: kapi-
 */

#ifndef LPUART_H_
#define LPUART_H_

void LPUART1_config(void);
void SendString(volatile char z[]);
uint8_t LPUART_Available(void);

void LPUART1_IRQHandler(void);

#endif /* LPUART_H_ */
