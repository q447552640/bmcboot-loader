/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-22 21:43:50
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-22 00:00:51
 * @Description: file content
 * @FilePath: \BootLoader\bsp_uart.h
 */
#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>
#include "gd32f4xx.h"

#define SerialPutString(x) Serial_PutString((uint8_t *)x)

/*
 * 根据逻辑图纸修改以下宏定义（非常重要）
 * USER_RCU_UART     uart时钟
 * USER_RCU_GPIO_TX  uart_tx 所在GPIO时钟
 * USER_RCU_GPIO_RX  uart_rx 所在GPIO时钟
 * USER_UART         uart 外设
 * USER_UART_TX_PORT uart_tx 所在GPIO的组
 * USER_UART_RX_PORT uart_rx 所在GPIO的组
 * USER_UART_TX_PIN  uart_tx GPIO 引脚
 * USER_UART_RX_PIN  uart_rx GPIO 引脚
 * USER_UART_GPIO_AF_TX uart_tx GPIO 复用配置
 * USER_UART_GPIO_AF_RX uart_rx GPIO 复用配置
 * USER_RCU_DMA      DMA时钟
 * USER_DMA          DMA 外设
 * USER_DMA_CHANNEL  uart 使用的DMA通道
 * USER_DMA_SUBPERI  uart 使用的DAM通道的子配置
 */

#define USER_RCU_UART RCU_UART4
#define USER_RCU_GPIO_TX RCU_GPIOC
#define USER_RCU_GPIO_RX RCU_GPIOD

#define USER_UART UART4

#define USER_UART_TX_PORT GPIOC
#define USER_UART_RX_PORT GPIOD

#define USER_UART_TX_PIN GPIO_PIN_12
#define USER_UART_RX_PIN GPIO_PIN_2

#define USER_UART_GPIO_AF_TX GPIO_AF_8
#define USER_UART_GPIO_AF_RX GPIO_AF_8

#define USER_RCU_DMA RCU_DMA0
#define USER_DMA DMA0
#define USER_DMA_CHANNEL DMA_CH0
#define USER_DMA_SUBPERI DMA_SUBPERI4


void InitSerial(void);
void ResetSerial(void);

uint8_t GetKey(void);

void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t *s);
uint32_t SerialKeyPressed(uint8_t *key);

void Int2Str(uint8_t *str, int32_t intnum);
int Str2Int(uint8_t *inputstr, int32_t *intnum);
void usart_start_receive_block(uint32_t bufferAddress, uint32_t size);
int GetUsartReceiveFinish(void);
#endif
