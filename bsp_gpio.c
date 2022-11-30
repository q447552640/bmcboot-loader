/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-24 23:09:29
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-25 00:18:25
 * @Description: file content
 * @FilePath: \BootLoader\bsp_gpio.c
 */

#include "bsp_gpio.h"
#include "gd32f4xx_gpio.h"

void InitGpio(void)
{
    rcu_periph_clock_enable(RCU_GPIOD);

    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

void PowerOnBmcPeriph(void)
{
    gpio_bit_set(GPIOD, GPIO_PIN_1);
}

void PowerOffBmcPeriph(void)
{
    gpio_bit_reset(GPIOD, GPIO_PIN_1);
}

void ResetGpio(void)
{
    gpio_bit_reset(GPIOD, GPIO_PIN_1);
    gpio_deinit(GPIOD);
    rcu_periph_clock_disable(RCU_GPIOD);
}
