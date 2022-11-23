/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-22 21:43:39
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-22 22:31:04
 * @Description: file content
 * @FilePath: \BootLoader\bsp_uart.c
 */
#include "bsp_uart.h"

#include "gd32f4xx_usart.h"             // GigaDevice::Device:StdPeripherals:USART

/**
 * @brief 初始化串口通信
 * 
 */
void InitSerial(void)
{
    rcu_periph_clock_enable(RCU_UART6);
	
	nvic_irq_enable(UART6_IRQn, 2U,0U);
	
	gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_8);	
	gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_7);
	
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);	
	gpio_af_set(GPIOE, GPIO_AF_8, GPIO_PIN_7 | GPIO_PIN_8);	
	
	usart_deinit(UART6);
	usart_baudrate_set(UART6, 115200U);
	usart_word_length_set(UART6,USART_WL_8BIT);
	usart_stop_bit_set(UART6,USART_STB_1BIT);
	usart_parity_config(UART6,USART_PM_NONE);
	
	usart_receive_config(UART6,USART_RECEIVE_ENABLE);
	usart_transmit_config(UART6, USART_TRANSMIT_ENABLE);
	
	// usart_interrupt_enable(UART6, USART_INTEN_RBNEIE);	
	usart_enable(UART6);
}

void ResetSerial(void)
{
    usart_disable(UART6);
    usart_deinit(UART6);
}

/**
 * @brief 从串口设备中获取一个字节
 * 
 * @param[out] key 存放字节数据的指针
 * @return uint32_t 1:读取出一个数据 0:无数据
 */
uint32_t SerialKeyPressed(uint8_t* key)
{
    if(SET == usart_flag_get(UART6, USART_FLAG_RBNE))
    {
        usart_flag_clear(UART6, USART_FLAG_RBNE);
        *key= (uint8_t)usart_data_receive(UART6);
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t GetKey(void)
{
    uint8_t key=0;
    while (1)
    {
        if(SerialKeyPressed(&key))
        {
            break;
        }
    }
    return key;
}

void SerialPutChar(uint8_t c)
{
    usart_data_transmit(UART6, c);

    while (RESET == usart_flag_get(UART6, USART_FLAG_TC))
    {
        //roll wait
    }
}

void Serial_PutString(uint8_t* s)
{
    while(*s !='\0')
    {
        SerialPutChar(*s);
        s++;
    }
}
