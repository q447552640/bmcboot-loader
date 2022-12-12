# GD32F4XX应用启动程序

GD32F4XX应用启动程序实现了GD32F450 MCU中IAP的BootLoader和在线更新应用的功能。

1. 在线升级功能：目前支持通过串口（RS232）的在线升级策略，使用的传输协议为 **YModem**。目前主流的终端软件均支持此协议传输。
2. 启动应用功能：BootLoader软件在启动时如果未发生人工干预，则会自启动应用软件。如果是在线升级应用成功后，则可通过输入指令启动应用软件。 

#### 软件架构
功能基于Keil5IDE开发，在文件系统上没有对源文件进行分层管理，在keil工程中文件组织如下图所示。软件功能需求跟简单，在开发时参考[使用USART进行STM32F4应用内编程](https://www.st.com/zh/embedded-software/stsw-stm32067.html)移植开发，并进行了裁剪和优化。 _目前软件没有身份授权协议，为了防止产品阶段MCU应用软件被恶意下载，因此不实现Ymodem发送文件功能。_ 
```
BMCBootLoader
        ┝----bsp
           ┝----bsp_uart.c    封装MCU串口初始化和收发功能
           ┝----bsp_uart.h    定义MCU串口接口和串口参数
           ┝----systick.c     封装系统时钟初始化和系统延时功能
           ┝----systick.h     定义系统时钟初始化和系统延时接口
           ┝----bsp_ocflash.c 封装MCU片内Flash初始化和操作功能
           ┝----bsp_ocflash.h 定义MCU片内Flash初始化和操作接口
           ┝----ymodem.c      封装Ymodem协议接收文件功能
           ┝----ymodem.h      定义Ymodem协议接收文件接口
           ┝----bsp_gpio.c    封装一些开关量控制功能 (可以根据需求裁剪)
           ┝----bsp_gpio.h    定义一些开关量控制接口 (可以根据需求裁剪)
           ┝----driverCrc16.c 封装直驱动CRC16-XModem校验功能
           ┝----driverCrc16.h 定义直驱动CRC16-XModem校验接口
        ┝----usrsrc
           ┝----menu.c    封装IAP用户菜单打印和功能响应功能
           ┝----menu.h    定义IAP用户菜单接口
           ┝----main.c    主函数
```
软件工作流程如下图所示，
![主流程](doc/Main%20Flow%20Chart.png)

