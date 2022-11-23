/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-22 21:03:02
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-23 12:47:25
 * @Description: file content
 * @FilePath: \BootLoader\main.c
 */
#include <gd32f4xx.h>
#include "bsp_uart.h"
#include "bsp_ocflash.h"
#include "systick.h"

extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

static int GetIAPIntper(void);
static void RunApp(void);
static int ExistApplication(void);

int main(void)
{
    
    //初始化Core时钟
    systick_config();
    //初始化串口
    InitSerial();

    if(GetIAPIntper() == 1)
    //如果获取到IAP请求中断则打印菜单进行IAP操作
    {

    }
    //如果无请求则跳转到APP启动
    else
    {
        RunApp();
    }

    return 0;
}

int GetIAPIntper(void)
{
    int delayCnt=1000;  //设置等待中断字符时间 约1s
    uint8_t key=0;

    do
    {
        if((1==SerialKeyPressed(&key)) && (0x1B == key))
        {
            Serial_PutString("Get ESC Now turn In IAP Progame\r\n");
            return 1;
        }
        delay_1ms(1);
    } while ((--delayCnt) >0);
    return 0;
}
/**
 * @brief 运行应用程序
 * 
 */
void RunApp(void)
{
    if(ExistApplication() != 0)
    {
        nvic_irq_disable(EXTI0_IRQn);

        JumpAddress=*(__IO uint32_t *)(APPLICATION_ADDRESS+4);  //application main address

        Jump_To_Application=(pFunction)JumpAddress;

        //初始化Application的栈指针
        __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

        Jump_To_Application();
    }
    else
    {
        Serial_PutString("Warning Not Application On BMC!!!\r\n");
    }
}

int ExistApplication(void)
{
    if((*(__IO uint32_t *)APPLICATION_ADDRESS) & 0x2FFE0000 == 0x20000000)
    {
        return 1;
    }
    return 0;
}
