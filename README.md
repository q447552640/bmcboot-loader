# GD32F4XX应用启动程序

GD32F4XX应用启动程序实现了GD32F450 MCU中IAP的BootLoader和在线更新应用的功能。

1. 在线升级功能：目前支持通过串口（RS232）的在线升级策略，使用的传输协议为 **YModem**。目前主流的终端软件均支持此协议传输。
2. 启动应用功能：BootLoader软件在启动时如果未发生人工干预，则会自启动应用软件。如果是在线升级应用成功后，则可通过输入指令启动应用软件。 

#### 优势/特点
1. 无需插入JTAG/SWD仿真器，MCU引出的串行通讯可直接升级软件。
2. 当前主流终端软件均可进行升级操作，无需开发控制软件。

#### 软件架构
功能基于Keil5IDE开发，在文件系统上没有对源文件进行分层管理，在keil工程中文件组织如下图所示。软件功能需求跟简单，在开发时参考[使用USART进行STM32F4应用内编程](https://www.st.com/zh/embedded-software/stsw-stm32067.html)移植开发，并进行了裁剪和优化。 _目前软件没有身份授权协议，为了防止产品阶段MCU应用软件被恶意下载，因此不实现Ymodem发送文件功能。_ 
```
BMCBootLoader
        ┝----bsp
           ┝----bsp_uart.c    封装MCU串口初始化和收发功能
           ┝----bsp_uart.h    定义MCU串口接口和串口参数 （可根据需要修改外设的宏定义）
           ┝----systick.c     封装系统时钟初始化和系统延时功能
           ┝----systick.h     定义系统时钟初始化和系统延时接口
           ┝----bsp_ocflash.c 封装MCU片内Flash初始化和操作功能
           ┝----bsp_ocflash.h 定义MCU片内Flash初始化和操作接口 （如需要适配其他MCU型号，需在此处修改宏定义）
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
软件工作流程如下图所示。
![主流程](doc/Main%20Flow%20Chart.png)

#### 发行说明
考虑到嵌入式软件对于硬件逻辑设计的强依赖性，因此在 **发布发行版本时不发布可执行文件** 。可下载Code source自行适配和编译。

##### [R-V1.0.0](https://gitee.com/VoldemortCat/bmcboot-loader/releases/tag/V1.0.0)
实现了IAP基本功能，并在GD32F450Z-EVAL评估板卡上进行了软件测试。
#### 构建工程
##### 编译环境
MDK-ARM Version 5.37.0.0或更高。 [MDK-ARM官方下载](https://www.keil.com/demo/eval/arm.htm)
##### 依赖开发包
1. GD32F4xx AddOn V3.0.0或更高 [GD32官网下载](https://gd32mcu.com/cn/download/7?kw=GD32F4)
##### 适配说明
 **建议适配时工程目标选择BootLoaderEval进行适配。** 
###### bsp_uart.h
根据不同的串口输出和引脚关系，修改该文件中的宏定义。定义引脚、串口和DMA时需要查阅用户手册和数据手册。需要注意：如果串口外设变更了，则DMA的宏定义是否比配串口TX。
###### bsp_ocflash.h
与STM32F4系列开发库接口不同，在擦写时需要Flash扇区号，所以在定义的时候增加了擦写扇的定义。当前应用程序可擦写扇范围为[2..11] (大小：992KB)，正常使用工况足够。如果空间不够使用，可根据需要自行拓展。（扇0、1为Bootloader程序烧录使用）
##### 编译
推荐使用MDK自带的Version5编译器（Version6语法检测更严格，一写库函数无法编译通过）。
#### 应用软件移植说明
应用软件移植已形成文档保存在代码库中，可单独下载[BMC传统应用迁移为IAP应用操作手册](https://gitee.com/VoldemortCat/bmcboot-loader/blob/master/doc/BMC%E4%BC%A0%E7%BB%9F%E5%BA%94%E7%94%A8%E8%BF%81%E7%A7%BB%E4%B8%BAIAP%E5%BA%94%E7%94%A8%E6%93%8D%E4%BD%9C%E6%89%8B%E5%86%8C.docx)。