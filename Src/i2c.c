/*
 * i2c.c
 *
 *  Created on: Jun 1, 2026
 *      Author: kapi-
 */

#include "main.h"
#include "stdint.h"
#include "i2c.h"

void I2C1_Init(void)
{
	//PB6 - I2C1 SCL - AF4
	//PB9 - I2C1 SDA - AF4

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->APB1ENR1 &= ~(RCC_APB1ENR1_I2C1EN);

	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

	GPIOB->MODER &= ~(GPIO_MODER_MODE6);
	GPIOB->MODER &= ~(GPIO_MODER_MODE9);

	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->MODER |= GPIO_MODER_MODE9_1;

	GPIOB->OTYPER |= GPIO_OTYPER_OT6;
	GPIOB->OTYPER |= GPIO_OTYPER_OT9;

	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_1;

	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2;

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD9);

	I2C1->CR1 &= ~(I2C_CR1_PE);
	I2C1->TIMINGR = 0x10909CEC;
	I2C1->CR1 |= I2C_CR1_PE;
}
void I2C1_Write_test(uint8_t data)
{
	uint32_t NBYTES = 1;
	uint32_t SADD = 0x78;

	uint32_t Value = 0;

	Value |= SADD;
	Value |= (NBYTES << 16);
	Value |= I2C_CR2_AUTOEND;
	Value |= I2C_CR2_START;

	I2C1->CR2 = Value;

	while(!(I2C1->ISR & I2C_ISR_TXIS))
	{

	}

	I2C1->TXDR = data;

	while(!(I2C1->ISR & I2C_ISR_STOPF))
	{

	}
	I2C1->ICR |= I2C_ICR_STOPCF;
}

void I2C1_WriteBuffer(uint32_t adr, uint8_t *buff, uint8_t length)
{
	uint32_t NBYTES = length;
	uint32_t SADD = adr;

	uint32_t Value = 0;

	Value |= SADD;
	Value |= (NBYTES << 16);
	Value |= I2C_CR2_AUTOEND;
	Value |= I2C_CR2_START;

	I2C1->CR2 = Value;

	uint8_t i = 0;

	while(i < length)
	{
		while(!(I2C1->ISR & I2C_ISR_TXIS))
			{

			}
		I2C1->TXDR = buff[i];
		i++;
	}

	while(!(I2C1->ISR & I2C_ISR_STOPF))
	{

	}
	I2C1->ICR |= I2C_ICR_STOPCF;

}

















