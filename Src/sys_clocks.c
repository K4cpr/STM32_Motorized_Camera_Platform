/*
 * sys_clocks.c
 *
 *  Created on: Dec 22, 2025
 *      Author: kapi-
 */

#include "main.h"
#include "sys_clocks.h"

volatile uint32_t Tick;

void SystemClockHSI16(void)
{

	RCC->CR |= RCC_CR_HSION;


	while ((RCC->CR & RCC_CR_HSIRDY) == 0) { }


	 RCC->CFGR &= ~RCC_CFGR_SW;
	 RCC->CFGR |= RCC_CFGR_SW_HSI;


	 while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) { }



}



void SystemClockPLL48(void)
{

	RCC->CR |= RCC_CR_HSION;


	while ((RCC->CR & RCC_CR_HSIRDY) == 0) { }



	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSI);

	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

	while (!(FLASH->ACR & FLASH_ACR_LATENCY_2WS));




	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= (12 << RCC_PLLCFGR_PLLN_Pos);

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLR);
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLR_0;


	RCC->CR |= RCC_CR_PLLON;


	while ((RCC->CR & RCC_CR_PLLRDY) == 0) { }



	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

	 RCC->CFGR |= RCC_CFGR_SW_1;
	 RCC->CFGR |= RCC_CFGR_SW_0;


	 while (!((RCC->CFGR & RCC_CFGR_SWS_1) && (RCC->CFGR & RCC_CFGR_SWS_0))) { }

	// RCC->CCIPR |= RCC_CCIPR_ADCSEL;

}

void SystemClockPLL80(void)
{


	RCC->CR |= RCC_CR_HSION;


	while ((RCC->CR & RCC_CR_HSIRDY) == 0) { }


	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSI);


	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

	while (!(FLASH->ACR & FLASH_ACR_LATENCY_4WS));


	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= (10 << RCC_PLLCFGR_PLLN_Pos);

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLR);


	RCC->CR |= RCC_CR_PLLON;


	while ((RCC->CR & RCC_CR_PLLRDY) == 0) { }



	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;



	 RCC->CFGR |= RCC_CFGR_SW_1;
	 RCC->CFGR |= RCC_CFGR_SW_0;


	 while (!((RCC->CFGR & RCC_CFGR_SWS_1) && (RCC->CFGR & RCC_CFGR_SWS_0))) { }



}

void SystemClockSetup(void)
{
//	SystemClockHSI16();
//	SystemClockPLL48();
	SystemClockPLL80();
	SysTick_Config(80000000 / 1000);
}

void SysTick_Handler(void)
{
	Tick++;

}


uint32_t GetSystemTick(void)
{
	return Tick;
}



void Delay(uint32_t Delay_ms)
{
	uint32_t StartTime = Tick;
	while(Tick < (StartTime + Delay_ms))
	{

	}
}








