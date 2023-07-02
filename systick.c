/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-26 11:49:23
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-29 10:56:00
 * @Description: file content
 * @FilePath: \undefinede:\worspace\arm\gd32\BMC\BootLoader\systick.c
 */
/*!
    \file  systick.c
    \brief the systick configuration file
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-07-28, V1.0.0, demo for GD32F30x
*/

#include "gd32f4xx.h"
#include "systick.h"

volatile static uint32_t delay;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for 10000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 10000U))
    {
        /* capture error */
        while (1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    delay = count * 10;

    while (0U != delay)
    {
    }
}

void delay_100us(uint32_t count)
{
    delay = count;
    while (0U != delay)
    {
    }
}

/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
    if (0U != delay)
    {
        delay--;
    }
}

void SysTick_Handler(void)
{
    delay_decrement();
}
