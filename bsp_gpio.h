/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-24 23:07:45
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-24 23:20:08
 * @Description: file content
 * @FilePath: \BootLoader\bsp_gpio.h
 */

#ifndef BSP_GPIO_H
#define BSP_GPIO_H

void InitGpio(void);

void PowerOnBmcPeriph(void);
void PowerOffBmcPeriph(void);
void ResetGpio(void);
#endif
