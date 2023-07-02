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
#include "driverCrc16.h"
#include "bootLoader.h"
#include "systick.h"

int main(void)
{
    SystemCoreClockUpdate();
    // 初始化Core时钟
    systick_config();
    InitCRC();
    // 初始化GPIO并打开V3.3外设供电
    InitGpio();
    int i = 10;
	  delay_1ms(500);
    SerialPutString("\r\nThanks for use this bootLoader\r\n");
    // 初始化串口
    InitSerial();
    while (i > 0)
    {
        PowerOnBmcPeriph();
        char str[64] = {0};
        sprintf(str, "\r\nWait for ESC key to enter IAP...remain %d s\r\n", i);
        SerialPutString(str);
        if (GetIAPIntper() == 1)
        // 如果获取到IAP请求中断则打印菜单进行IAP操作
        {
            Flash_IF_Init();
            PrintMenu();
            break;
        }
        delay_1ms(500);
        PowerOffBmcPeriph();
        delay_1ms(500);
        i--;
    }
    if (i <= 0)
    {
        // 如果无请求则跳转到APP启动
        SerialPutString("Run App...\r\n");
        RunApp();
    }

    while (1)
    {
    }
}
