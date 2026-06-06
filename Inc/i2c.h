/*
 * i2c.h
 *
 *  Created on: Jun 1, 2026
 *      Author: kapi-
 */

#ifndef I2C_H_
#define I2C_H_

void I2C1_Init(void);
void I2C1_Write_test(uint8_t data);
void I2C1_WriteBuffer(uint32_t adr, uint8_t *buff, uint8_t length);
uint8_t I2C1_Scanner(uint32_t adr);

#endif /* I2C_H_ */
