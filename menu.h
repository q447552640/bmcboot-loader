/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-24 10:03:22
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-24 10:32:34
 * @Description: file content
 * @FilePath: \BootLoader\menu.h
 */

#ifndef MENU_H_
#define MENU_H_

#include <string.h>
#include "ymodem.h"
#include "bsp_ocflash.h"
#include "bsp_uart.h"



void PrintMenu(void);

void LoadRunApplication(void);

#endif
