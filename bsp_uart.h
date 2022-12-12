/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-22 21:43:50
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-26 18:39:20
 * @Description: file content
 * @FilePath: \BootLoader\bsp_uart.h
 */
#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>
#include "gd32f4xx.h"

#define SerialPutString(x) Serial_PutString((uint8_t *)x)

#ifndef BOARD_EVAL
    #define USER_RCU_UART        RCU_UART6
    #define USER_RCU_GPIO_TX     RCU_GPIOE
    #define USER_RCU_GPIO_RX     RCU_GPIOE

    #define USER_UART            UART6

    #define USER_UART_TX_PORT    GPIOE
    #define USER_UART_RX_PORT    GPIOE

    #define USER_UART_TX_PIN     GPIO_PIN_8
    #define USER_UART_RX_PIN     GPIO_PIN_7
		
    #define USER_UART_GPIO_AF_TX   GPIO_AF_8
		#define USER_UART_GPIO_AF_RX   GPIO_AF_8
		
		#define USER_RCU_DMA				RCU_DMA0
		#define USER_DMA						DMA0
		#define USER_DMA_CHANNEL		DMA_CH3
		#define USER_DMA_SUBPERI		DMA_SUBPERI5
#else
    #define USER_RCU_UART        RCU_USART0
    #define USER_RCU_GPIO_TX     RCU_GPIOA
    #define USER_RCU_GPIO_RX     RCU_GPIOA

    #define USER_UART            USART0

    #define USER_UART_TX_PORT    GPIOA
    #define USER_UART_RX_PORT    GPIOA

    #define USER_UART_TX_PIN     GPIO_PIN_9
    #define USER_UART_RX_PIN     GPIO_PIN_10

    #define USER_UART_GPIO_AF_TX   GPIO_AF_7
		#define USER_UART_GPIO_AF_RX   GPIO_AF_7
		
		#define USER_RCU_DMA				RCU_DMA1
		#define USER_DMA						DMA1
		#define USER_DMA_CHANNEL		DMA_CH2
		#define USER_DMA_SUBPERI		DMA_SUBPERI4
#endif

void InitSerial(void);
void ResetSerial(void);

uint8_t GetKey(void);

void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t* s);
uint32_t SerialKeyPressed(uint8_t* key);


int Int2Str(uint8_t* str, int32_t intnum);
int Str2Int(uint8_t *inputstr, int32_t *intnum);
void usart_start_receive_block(uint32_t bufferAddress, uint32_t size);
int GetUsartReceiveFinish(void);
#endif
