/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-23 12:18:51
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-23 22:20:10
 * @Description: file content
 * @FilePath: \BootLoader\bsp_ocflash.h
 */
#ifndef BSP_OCFLASH_H
#define BSP_OCFLASH_H

#include <stdint.h>


#define APPLICATION_ADDRESS     (uint32_t)0x08008000

#define APPLICATION_START_SECTOR    2
#define APPLICATION_END_SECTOR      11

typedef void (*pFunction)(void);

void Flash_IF_Init(void);

int Flash_IF_App_Erase(int StartSectorId, int EndSectorId);
#endif
