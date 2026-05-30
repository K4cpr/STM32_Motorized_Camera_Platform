/*
 * motor.h
 *
 *  Created on: May 26, 2026
 *      Author: kapi-
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include <stdint.h>
#include "main.h"

void Motor_Init(void);
void Motor_Left(void);
void Motor_Right(void);
void Motor_Off(void);
uint16_t Speed(uint8_t speed);

#endif /* MOTOR_H_ */
