/*
 * oled.h
 *
 *  Created on: Jun 2, 2026
 *      Author: kapi-
 */

#ifndef OLED_H_
#define OLED_H_

void OLED_Command(uint8_t cmd);
void OLED_Command2(uint8_t cmd, uint8_t value);
void OLED_Init(void);
void OLED_DataBuffer(uint8_t *data, uint8_t length);
void OLED_Clear(void);
void OLED_Fill(void);
void OLED_SetCursor(uint8_t page, uint8_t column);
void OLED_WriteChar(char c);
void OLED_WriteString(const char *z);
void OLED_WriteInt(uint16_t num);

#endif /* OLED_H_ */
