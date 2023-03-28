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
#ifndef BOARD_EVAL
#include "bsp_gpio.h"
#endif
#include "bsp_uart.h"
#include "driverCrc16.h"
#include "bootLoader.h"
#include "systick.h"

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


