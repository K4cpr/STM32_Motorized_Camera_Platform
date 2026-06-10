/*
 * encoder.h
 *
 *  Created on: Jun 6, 2026
 *      Author: kapi-
 */

#ifndef ENCODER_H_
#define ENCODER_H_

extern uint8_t Speed_Percent;

void encoder_init(void);
uint16_t encoder_GetResult(void);
void encoder_reset(void);
int16_t encoder_GetChange(void);
int32_t encoder_GetPositionAngle(void);
int32_t encoder_GetSpeed(void);
void encoder_Update(void);
void OLED_ShowEncoderData(void);
int32_t encoder_GetSpeedint(void);
void encoder_ResetSpeed(void);

#endif /* ENCODER_H_ */
