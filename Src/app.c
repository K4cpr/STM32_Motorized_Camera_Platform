/*
 * app.c
 *
 *  Created on: Jun 8, 2026
 *      Author: kapi-
 */


#include "main.h"
#include "sys_clocks.h"
#include "lpuart.h"
#include "uart_protocol.h"
#include "state_machine.h"
#include "motor.h"
#include "tim.h"
#include "i2c.h"
#include "oled.h"
#include "joy.h"
#include "encoder.h"

volatile uint32_t encoder_cnt;
//volatile int32_t encoder_angle;
volatile int32_t encoder_rpm;

void App_Init(void)
{
    SystemClockSetup();


    Motor_Init();

    Tim15Init();
    Tim15_Start();

    LPUART1_config();

    StateMachine_Init();

    I2C1_Init();

    OLED_Init();
    OLED_Clear();

    joy_init();

    encoder_init();
}

void App_Run(void)
{
    encoder_Update();
    OLED_ShowEncoderData();

//    encoder_angle = encoder_GetPositionAngle();
//    encoder_rpm = encoder_GetSpeed();

    UART_Protocol_Process();
    StateMachine_Run();
}
