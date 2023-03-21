/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-22 21:43:39
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-21 23:31:45
 * @Description: file content
 * @FilePath: \BootLoader\bsp_uart.c
 */
#include <stdint.h>
#include "bsp_uart.h"

#include "gd32f4xx_usart.h" // GigaDevice::Device:StdPeripherals:USART
#include "gd32f4xx_dma.h"               // GigaDevice::Device:StdPeripherals:DMA


#define IS_AF(c) ((c >= 'A') && (c <= 'F'))
#define IS_af(c) ((c >= 'a') && (c <= 'f'))
#define IS_09(c) ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c) IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c) IS_09(c)
#define CONVERTDEC(c) (c - '0')

#define CONVERTHEX_alpha(c) (IS_AF(c) ? (c - 'A' + 10) : (c - 'a' + 10))
#define CONVERTHEX(c) (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

/**
 * @brief 初始化串口通信
 *
 */
void InitSerial(void)
{
  rcu_periph_clock_enable(USER_RCU_GPIO_TX);
	rcu_periph_clock_enable(USER_RCU_GPIO_RX);
  rcu_periph_clock_enable(USER_RCU_UART);

  // nvic_irq_enable(UART6_IRQn, 2U,0U);

  gpio_mode_set(USER_UART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USER_UART_TX_PIN);
  gpio_mode_set(USER_UART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USER_UART_RX_PIN);

  gpio_output_options_set(USER_UART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USER_UART_TX_PIN);
  gpio_af_set(USER_UART_TX_PORT, USER_UART_GPIO_AF_TX, USER_UART_TX_PIN);
	gpio_af_set(USER_UART_RX_PORT, USER_UART_GPIO_AF_RX, USER_UART_RX_PIN);
	
	  usart_deinit(USER_UART);

  usart_baudrate_set(USER_UART, 115200U);
  usart_word_length_set(USER_UART, USART_WL_8BIT);
  usart_stop_bit_set(USER_UART, USART_STB_1BIT);
  usart_parity_config(USER_UART, USART_PM_NONE);

  usart_hardware_flow_rts_config(USER_UART, USART_RTS_DISABLE);
  usart_hardware_flow_cts_config(USER_UART, USART_CTS_DISABLE);


  usart_receive_config(USER_UART, USART_RECEIVE_ENABLE);
  usart_transmit_config(USER_UART, USART_TRANSMIT_ENABLE);

  // usart_interrupt_enable(USER_UART, USART_INTEN_RBNEIE);
  usart_enable(USER_UART);
}

void ResetSerial(void)
{
  usart_deinit(USER_UART);
  gpio_deinit(USER_UART_TX_PORT);
	gpio_deinit(USER_UART_RX_PORT);
	dma_deinit(USER_DMA, USER_DMA_CHANNEL);
	
	rcu_periph_clock_disable(USER_RCU_DMA);
}

/**
 * @brief 从串口设备中获取一个字节
 *
 * @param[out] key 存放字节数据的指针
 * @return uint32_t 1:读取出一个数据 0:无数据
 */
uint32_t SerialKeyPressed(uint8_t *key)
{
	usart_dma_receive_config(USER_UART, USART_DENR_DISABLE);
	
  if (SET == usart_flag_get(USER_UART, USART_FLAG_RBNE))
  {
    usart_flag_clear(USER_UART, USART_FLAG_RBNE);
    *key = (uint8_t)usart_data_receive(USER_UART);
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t GetKey(void)
{
  uint8_t key = 0;
  while (1)
  {
    if (SerialKeyPressed(&key))
    {
      break;
    }
  }
  return key;
}

void SerialPutChar(uint8_t c)
{
  usart_data_transmit(USER_UART, c);

  while (RESET == usart_flag_get(USER_UART, USART_FLAG_TC))
  {
    // roll wait
  }
}

void Serial_PutString(uint8_t *s)
{
  while (*s != '\0')
  {
    SerialPutChar(*s);
    s++;
  }
}

int Int2Str(uint8_t *str, int32_t intnum)
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;

    intnum = intnum % Div;
    Div /= 10;
    if ((str[j - 1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
}

/**
 * @brief Intel String trans to Integer
 * @authors STM
 * @param inputstr
 * @param intnum
 * @return int
 */
int Str2Int(uint8_t *inputstr, int32_t *intnum)
{
  uint32_t i = 0, res = 0;
  uint32_t val = 0;

  if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
  {
    if (inputstr[2] == '\0')
    {
      return 0;
    }
    for (i = 2; i < 11; i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1; */
        res = 1;
        break;
      }
      if (ISVALIDHEX(inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* over 8 digit hex --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }
  else /* max 10-digit decimal input */
  {
    for (i = 0; i < 11; i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1 */
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
      {
        val = val << 10;
        *intnum = val;
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
      {
        val = val << 20;
        *intnum = val;
        res = 1;
        break;
      }
      else if (ISVALIDDEC(inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 10 digit decimal --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }

  return res;
}

/**
 * @description: 启动DMA接收UART RX消息
 * @param {uint32_t} bufferAddress
 * @param {uint32_t} size
 * @return {*}
 */
void usart_start_receive_block(uint32_t bufferAddress, uint32_t size)
{
	dma_single_data_parameter_struct dma_data_paramter;
	
	rcu_periph_clock_enable(USER_RCU_DMA);
	
	dma_deinit(USER_DMA, USER_DMA_CHANNEL);
	
	dma_data_paramter.periph_addr=(uint32_t)(&USART_DATA(USER_UART));
	dma_data_paramter.periph_inc=DMA_PERIPH_INCREASE_DISABLE;
	dma_data_paramter.memory0_addr=bufferAddress;
	dma_data_paramter.memory_inc=DMA_MEMORY_INCREASE_ENABLE;
	dma_data_paramter.periph_memory_width=DMA_PERIPH_WIDTH_8BIT;
	dma_data_paramter.direction=DMA_PERIPH_TO_MEMORY;
	dma_data_paramter.priority=DMA_PRIORITY_HIGH;
	dma_data_paramter.number=size;
	dma_data_paramter.circular_mode=DMA_CIRCULAR_MODE_DISABLE;
	
	dma_single_data_mode_init(USER_DMA, USER_DMA_CHANNEL, dma_data_paramter);
	
	dma_circulation_disable(USER_DMA, USER_DMA_CHANNEL);
	dma_channel_subperipheral_select(USER_DMA, USER_DMA_CHANNEL, USER_DMA_SUBPERI);
	
	dma_channel_enable(USER_DMA, USER_DMA_CHANNEL);
	
	usart_dma_receive_config(USER_UART, USART_DENR_ENABLE);
	/*
	usart_interrupt_enable(USER_UART, USART_INTEN_IDLEIE);
	dma_interrupt_enable(USER_DMA, USER_DMA_CHANNEL, DMA_CHXCTL_FTFIE);
	
	nvic_irq_enable(USART0_IRQn, 0,0);
	nvic_irq_enable(USER_DMA_Channel2_IRQn,1,0);
	*/
}

/**
 * @description: 串口 RX是否结束
 * @return {*}
 */
int GetUsartReceiveFinish(void)
{
	int result=0;
  //在一个帧时间内，RX引脚检测到空闲状态，会将IDLEF置为 1
	if(usart_flag_get(USER_UART, USART_FLAG_IDLEF) ==SET)
	{
    //根据手册
    //通过读寄存器操作，清除IDLEF位
		usart_data_receive(USER_UART);
		result= 1;
	}
  //当DMA数据满时，该状态标志有效
	if(dma_flag_get(USER_DMA, USER_DMA_CHANNEL, DMA_INTF_FTFIF)==SET)
	{
    //清除状态标志
		dma_flag_clear(USER_DMA, USER_DMA_CHANNEL, DMA_INTF_FEEIF);
		result= 2;
	}
	return result;
}
