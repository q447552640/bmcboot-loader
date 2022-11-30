/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-30 19:01:15
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-30 19:32:24
 * @Description: file content
 * @FilePath: \BootLoader\driverCrc16.h
 */
#ifndef Driver_CRC16_H
#define Driver_CRC16_H

#include <stdint.h>

#define CRC_WIDTH 16
#define CRC_PLOY    0x1021
#define CRC_INIT    0x0000

void InitCRC(void);
uint16_t CalcuCRC(char* buff, int len);

#endif //Driver_CRC16_H
