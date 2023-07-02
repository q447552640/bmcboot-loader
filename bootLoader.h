/*
 * @Author: jimma0312 jimma0312@outlook.com
 * @Date: 2023-03-28 19:09:35
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-28 19:12:41
 * @FilePath: .\bootLoader.h
 * @Description: 应用程序启动模块 定义
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H

int GetIAPIntper(void);
void RunApp(void);
int ExistApplication(void);
void LoadRunApplication(void);

#endif // BOOTLOADER_H
