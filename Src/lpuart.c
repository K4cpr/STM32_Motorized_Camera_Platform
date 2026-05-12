/*
 * lpuart.c
 *
 *  Created on: Feb 23, 2026
 *      Author: kapi-
 */

#include "main.h"
#include "lpuart.h"
#include <string.h>



volatile uint8_t BuffTx[100];
volatile char BuffRx[100];
volatile int tx_busy;
volatile uint32_t tx_index = 0;
volatile uint32_t rx_index = 0;

volatile uint8_t rx_ready = 0;


void LPUART1_config(void)
{
	//PC0 PLUART1 RX
	//PC1 LPUART1 TX

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
	//RX
	GPIOC->MODER &= ~(GPIO_MODER_MODE0_0);
	GPIOC->AFR[0] |= GPIO_AFRL_AFSEL0_3;

	//TX
	GPIOC->MODER &= ~(GPIO_MODER_MODE1_0);
	GPIOC->AFR[0] |= GPIO_AFRL_AFSEL1_3;

	LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
	LPUART1->CR1 |= USART_CR1_RXNEIE;

	uint32_t BRR_VALUE = 256*(80000000/115200);
	LPUART1->BRR = BRR_VALUE;

	if(BRR_VALUE < 0x300 || BRR_VALUE > 0xFFFFF)
		{
			while(1){};
		}

	NVIC_SetPriority(LPUART1_IRQn, 0);
	NVIC_EnableIRQ(LPUART1_IRQn);

}

void LPUART1_IRQHandler(void)
{
	//TX
	if(LPUART1->ISR & USART_ISR_TXE)
	{
		if(tx_busy == 1)
		{
				LPUART1->TDR = BuffTx[tx_index];
				tx_index ++;
				if(BuffTx[tx_index] == '\0')
				{
					LPUART1->CR1 &= ~(USART_CR1_TXEIE);
					tx_busy = 0;
					tx_index = 0;
				}

		}
	}

	//RX
	volatile char word;
	if(LPUART1->ISR & USART_ISR_RXNE)
	{
		word = LPUART1->RDR;

		if(word == '\r' || word == '\n')
		{
			BuffRx[rx_index] = '\0';
			rx_ready = 1;
			rx_index = 0;
		}
		else
		{
			BuffRx[rx_index] = word;
			rx_index++;
		}
	}
}

void SendString(volatile char z[])
{
	while(tx_busy == 1);

	int copy_index = 0;

	while(z[copy_index] != '\0')
	{
		BuffTx[copy_index] = z[copy_index];
		copy_index ++;
	}
	BuffTx[copy_index] = '\0';
	tx_busy = 1;
	tx_index = 0;

	LPUART1->CR1 |= USART_CR1_TXEIE;

}

uint8_t LPUART_Available(void)
{
	if(rx_ready == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}




