/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-30 19:01:06
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-30 19:33:29
 * @Description: file content
 * @FilePath: \BootLoader\driverCrc16.c
 */
#include "driverCrc16.h"

static uint16_t Crc_Table[256] = {0};

void InitCRC(void)
{
    int index = 0;
    int bit_cnt = 0;
    uint32_t temp = 0;

    for (index = 0; index < 256; index++)
    {
        temp = 0;
        for (bit_cnt = 8; bit_cnt > 0; bit_cnt--)
        {
            if ((index >> (bit_cnt - 1) ^ temp >> (CRC_WIDTH - 1)) & 0x1)
            {
                temp = (temp << 1) ^ CRC_PLOY;
            }
            else
            {
                temp <<= 1;
            }
        }

        Crc_Table[index] = (uint16_t)temp;
    }
}

uint16_t CalcuCRC(char *buff, int len)
{
    char *tempPtr = buff;
    int reg = CRC_INIT;
    int i = 0;

    for (i = 0; i < len; i++)
    {
        reg = (reg << 8) ^ Crc_Table[((reg >> (CRC_WIDTH - 8)) & 0xFF) ^ *tempPtr];
        tempPtr++;
    }
    return (uint16_t)(reg & 0xFFFF);
}
