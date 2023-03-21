/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-23 12:18:51
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-15 23:54:33
 * @Description: 声明MCU内部Flash操作接口和配置空间
 * @FilePath: \BootLoader\bsp_ocflash.h
 */
#ifndef BSP_OCFLASH_H
#define BSP_OCFLASH_H

#include <gd32f4xx.h>
#include <stdint.h>

/*应用软件的起始地址*/
#define APPLICATION_ADDRESS     (uint32_t)0x08008000
/*应用软件的最大结束地址*/
#define USER_FLASH_END_ADDRESS  (uint32_t)0x0807FFFF

/*应用软件最大的空间大小*/
#define USER_FLASH_SIZE (USER_FLASH_END_ADDRESS-APPLICATION_ADDRESS+1)

/*应用软件在MCU的Flash起始扇区*/
#define APPLICATION_START_SECTOR    2
/*应用软件在MCU中可使用的最后一个扇区*/
#define APPLICATION_END_SECTOR      7

typedef void (*pFunction)(void);

/**
 * @description: 初始化MCU内部Flash操作模块
 * @return {*}
 */
void Flash_IF_Init(void);
/**
 * @description: 擦除MCU内部Flash
 * @param {int} StartSectorId 起始扇区
 * @param {int} EndSectorId 结束扇区
 * @return {int} 0 擦除成功； -1 擦除失败
 */
int Flash_IF_App_Erase(int StartSectorId, int EndSectorId);
/**
 * @description: 按照4字节单位将缓存区数据烧写到Flash
 * 
 * @param address [in out] 传输地址的指针
 * @param buffer [in] 数据缓存区
 * @param length [in] 数据缓存区长度 
 * @return int 0 正确 -1 烧写错误 -2 回读错误
 */
int Flash_IF_Write(__IO uint32_t *address, uint32_t *buffer, int length);
#endif
