/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-24 10:03:28
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-25 15:19:15
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
    uint8_t printProgramMenu=1;

    SerialPutString("=========================================================\r\n");
    SerialPutString("=                                                       =\r\n");
    SerialPutString("=  BMC GD32F4XX In-Application Programming Apllication  =\r\n");
    SerialPutString("=                       V 0.0.1                         =\r\n");
    SerialPutString("=========================================================\r\n");

    while (1)
    {
        if(printProgramMenu!=0)
        {
            printProgramMenu=0;
        SerialPutString("========================Main Menu========================\r\n");
        SerialPutString("== Download Image To the GD32F4xx Internal Flash--------1\r\n");
        SerialPutString("== Exec New Program-------------------------------------2\r\n");
        SerialPutString("=========================================================\r\n");
        }

        key = GetKey();
				//usart_start_receive_block((uint32_t)&key,1);
				
				while(GetUsartReceiveFinish()==0)
				{}

        if (key == 0x31)
        // Download Image & program in flash
        {
            SerialDownLoad();
            printProgramMenu=1;
        }
        else if (key == 0x32)
        {
						SerialPutString("======Run App======\r\n");
            LoadRunApplication();
            printProgramMenu=1;
						return ;
        }
        else if(key == 0x1B)
        {

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

    SerialPutString(" Waiting for file to be sent ...\r\n");
    Size=Ymodem_Receive(tab_1024);

    if(Size>0)
    {
        SerialPutString(" Programming Completed Successfully!\r\n Name: ");
        SerialPutString(FileName);
        Int2Str(Number, Size);
        SerialPutString(" Size: ");
        SerialPutString(Number);
        SerialPutString("Bytes\r\n");
        SerialPutString("--------------------------\r\n");
    }
    else
    {
        SerialPutString(" Warning Receive File failed!!\r\n");
    }
}

void LoadRunApplication(void)
{
    PowerOffBmcPeriph();
    ResetSerial();
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
