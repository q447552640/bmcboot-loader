/*
 * @Author: jimma0312 jimma0312@outlook.com
 * @Date: 2023-03-28 19:09:24
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-28 19:12:52
 * @FilePath: .\bootLoader.c
 * @Description: 应用程序启动模块 实现
 */
#include "gd32f4xx.h" // Device header

#include <stdio.h>
#include <stdint.h>
#include "bootLoader.h"

#ifndef BOARD_EVAL
#include "bsp_gpio.h"
#endif
#include "bsp_uart.h"
#include "bsp_ocflash.h"
#include "systick.h"

static pFunction Jump_To_Application;
static uint32_t JumpAddress;

int GetIAPIntper(void)
{
    int delayCnt = 1000; // 设置等待中断字符时间 约1s
    uint8_t key = 0;

    do
    {
        if ((1 == SerialKeyPressed(&key)) && (0x1B == key))
        {
            Serial_PutString("Get ESC Now turn In IAP Progame\r\n");
            return 1;
        }
        delay_1ms(1);
    } while ((--delayCnt) > 0);
    return 0;
}
/**
 * @brief 运行应用程序
 *
 */
void RunApp(void)
{
    if (ExistApplication() != 0)
    {
        LoadRunApplication();
    }
    else
    {
        Serial_PutString("Warning Not Application On BMC!!!\r\n");
    }
}

int ExistApplication(void)
{
    if (((*(__IO uint32_t *)APPLICATION_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        return 1;
    }
    return 0;
}

void LoadRunApplication(void)
{
    ResetSerial();
    PowerOffBmcPeriph();
    ResetGpio();
    delay_1ms(500);

    nvic_irq_disable(EXTI0_IRQn);
    //__set_FAULTMASK(1);

    JumpAddress = *(__IO uint32_t *)(APPLICATION_ADDRESS + 4); // application main address

    Jump_To_Application = (pFunction)JumpAddress;

    // 初始化Application的栈指针
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

    Jump_To_Application();
}
