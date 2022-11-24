/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-24 10:03:28
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-24 23:21:38
 * @Description: file content
 * @FilePath: \BootLoader\menu.c
 */

#include "gd32f4xx.h"
#include "menu.h"
#include "bsp_gpio.h"
#include "systick.h"

static pFunction Jump_To_Application;
static uint32_t JumpAddress;

static uint8_t tab_1024[PACKET_DATA_LENGTH] = {0};

void SerialDownLoad(void);

/**
 * @brief Menu的主函数
 *
 */
void PrintMenu(void)
{
    uint8_t key = 0;

    SerialPutString("\r\n=========================================================\r\n");
    SerialPutString("\r\n=                                                       =\r\n");
    SerialPutString("\r\n=  BMC GD32F4XX In-Application Programming Apllication  =\r\n");
    SerialPutString("\r\n=                       V 0.0.1                         =\r\n");
    SerialPutString("\r\n=========================================================\r\n");

    while (1)
    {
        SerialPutString("\r\n========================Main Menu========================\r\n");
        SerialPutString("\r\n== Download Image To the GD32F4xx Internal Flash--------1\r\n");
        SerialPutString("\r\n== Exec New Program-------------------------------------2\r\n");
        SerialPutString("\r\n=========================================================\r\n");

        key = GetKey();

        if (key == 0x31)
        // Download Image & program in flash
        {
            SerialDownLoad();
        }
        else if (key == 0x32)
        {
            LoadRunApplication();
						return ;
        }
        else
        {
            SerialPutString("Invalid Num ! ==> The number should be either1,2\r\n");
        }
    }
}

void SerialDownLoad(void)
{
    uint8_t Number[10]={0};
    int32_t Size=0;

    SerialPutString("\r\n Waiting for file to be sent ...\r\n");
    Size=Ymodem_Receive(tab_1024);

    if(Size>0)
    {
        SerialPutString("\r\n Programming Completed Successfully!\r\n Name: ");
        SerialPutString(FileName);
        Int2Str(Number, Size);
        SerialPutString("\r\n Size: ");
        SerialPutString(Number);
        SerialPutString("Bytes\r\n");
        SerialPutString("--------------------------\r\n");
    }
    else
    {
        SerialPutString("\r\n Warning Receive File failed!!\r\n");
    }
}

void LoadRunApplication(void)
{
    PowerOffBmcPeriph();
    ResetSerial();
    ResetGpio();

    delay_1ms(500);

    nvic_irq_disable(EXTI0_IRQn);

    JumpAddress = *(__IO uint32_t *)(APPLICATION_ADDRESS + 4); // application main address

    Jump_To_Application = (pFunction)JumpAddress;

    // 初始化Application的栈指针
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

    Jump_To_Application();
}
