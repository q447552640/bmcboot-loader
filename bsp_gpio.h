/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-24 23:07:45
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-15 23:48:01
 * @Description: 声明GPIO接口
 * @FilePath: \BootLoader\bsp_gpio.h
 */

#ifndef BSP_GPIO_H
#define BSP_GPIO_H

/**
 * @description: 初始化GPIO外设
 * @return {*}
 */
void InitGpio(void);
/**
 * @description: 打开BMC需要使用的基础外设电源
 * @return {*}
 */
void PowerOnBmcPeriph(void);
/**
 * @description: 关闭BMC需要使用的基础外设电源
 * @return {*}
 */
void PowerOffBmcPeriph(void);
/**
 * @description: 重置（关闭）GPIO外设
 * @return {*}
 */
void ResetGpio(void);
#endif
