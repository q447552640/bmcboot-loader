/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-22 21:43:50
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-23 10:55:54
 * @Description: file content
 * @FilePath: \BootLoader\bsp_uart.h
 */
#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>

#define SerialPutString(x) Serial_PutString((uint8_t *)x)

void InitSerial(void);
void ResetSerial(void);

uint8_t GetKey(void);

void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t* s);
uint32_t SerialKeyPressed(uint8_t* key);

#endif
