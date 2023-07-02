/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-24 23:09:29
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-15 23:36:57
 * @Description: 实现板卡GPIO控制
 * @FilePath: \BootLoader\bsp_gpio.c
 */

#include "bsp_gpio.h"
#include "gd32f4xx_gpio.h"

void InitGpio(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
}

void PowerOnBmcPeriph(void)
{
    gpio_bit_set(GPIOA, GPIO_PIN_0);
}

void PowerOffBmcPeriph(void)
{
    gpio_bit_reset(GPIOA, GPIO_PIN_0);
}

void ResetGpio(void)
{
    gpio_bit_reset(GPIOA, GPIO_PIN_0);
    gpio_deinit(GPIOA);
    rcu_periph_clock_disable(RCU_GPIOA);
}
