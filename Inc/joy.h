/*
 * joy.h
 *
 *  Created on: Jun 5, 2026
 *      Author: kapi-
 */

#ifndef JOY_H_
#define JOY_H_

#include <string.h>

extern uint8_t Speed_Percent;

void joy_init(void);
void Joy_Read(void);

void up(void);
void down(void);
void right(void);
void left(void);
void ok(void);

#endif /* JOY_H_ */
