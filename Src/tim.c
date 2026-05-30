/*
 * tim.c
 *
 *  Created on: Apr 29, 2026
 *      Author: kapi-
 */

#include "main.h"
#include "sys_clocks.h"
#include "tim.h"
#include "motor.h"

// TIM15 CHANNEL 1 - PWM


void Tim15Init(void)
{
	RCC->APB2ENR &= ~(RCC_APB2ENR_TIM15EN);
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;

	TIM15->CNT = 0;
}

void Tim15_Start(void)
{
	TIM15->PSC = 79;
	TIM15->ARR = 999;

	TIM15->CCR1 = Speed(70);

	TIM15->CCMR1 &= ~(TIM_CCMR1_OC1M);
	TIM15->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	TIM15->CCMR1 |= TIM_CCMR1_OC1PE;

	TIM15->CCER &= ~(TIM_CCER_CC1P);
	TIM15->CCER |= TIM_CCER_CC1E;

	TIM15->BDTR |= TIM_BDTR_MOE;
	TIM15->CR1  |= TIM_CR1_CEN;
}
