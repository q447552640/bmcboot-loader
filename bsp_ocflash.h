/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-23 12:18:51
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-24 22:26:37
 * @Description: file content
 * @FilePath: \BootLoader\bsp_ocflash.h
 */
#ifndef BSP_OCFLASH_H
#define BSP_OCFLASH_H

#include <gd32f4xx.h>
#include <stdint.h>


#define APPLICATION_ADDRESS     (uint32_t)0x08008000
#define USER_FLASH_END_ADDRESS  (uint32_t)0x0807FFFF

#define USER_FLASH_SIZE (USER_FLASH_END_ADDRESS-APPLICATION_ADDRESS+1)

#define APPLICATION_START_SECTOR    2
#define APPLICATION_END_SECTOR      7

typedef void (*pFunction)(void);

void Flash_IF_Init(void);

int Flash_IF_App_Erase(int StartSectorId, int EndSectorId);
/**
 * @brief 按照4字节单位将缓存区数据烧写到Flash
 * 
 * @param address [in out] 传输地址的指针
 * @param buffer [in] 数据缓存区
 * @param length [in] 数据缓存区长度 
 * @return int 0 正确 -1 烧写错误 -2 回读错误
 */
int Flash_IF_Write(__IO uint32_t *address, uint32_t *buffer, int length);
#endif
