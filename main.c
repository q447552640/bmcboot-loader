/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-22 21:03:02
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-30 20:58:26
 * @Description: file content
 * @FilePath: \BootLoader\main.c
 */
#include <gd32f4xx.h>
#include "menu.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_ocflash.h"
#include "driverCrc16.h"
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
    InitCRC();
#ifndef BOARD_EVAL
    //初始化GPIO并打开V3.3外设供电
    InitGpio();
    PowerOnBmcPeriph();
    delay_1ms(10);
#endif
    //初始化串口
    InitSerial();
	
		SerialPutString("\r\nThanks for use this bootLoader\r\n");

    if(GetIAPIntper() == 1)
	// if(1)
    //如果获取到IAP请求中断则打印菜单进行IAP操作
    {
        Flash_IF_Init();
        PrintMenu();
    }
    //如果无请求则跳转到APP启动
    else
    {
		SerialPutString("Run App...\r\n");
        RunApp();
    }

	while(1)
	{
	}	
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
        LoadRunApplication();
    }
    else
    {
        Serial_PutString("Warning Not Application On BMC!!!\r\n");
    }
}

int ExistApplication(void)
{
    if(((*(__IO uint32_t *)APPLICATION_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        return 1;
    }
    return 0;
}
