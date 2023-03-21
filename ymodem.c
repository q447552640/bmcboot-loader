/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-23 23:29:58
 * @LastEditors: jimma0312 jimma0312@outlook.com
 * @LastEditTime: 2023-03-21 22:19:21
 * @Description: file content
 * @FilePath: \BootLoader\ymodem.c
 */
#include "ymodem.h"
#include "bsp_ocflash.h"
#include "bsp_uart.h"
#include "driverCrc16.h"
#include "systick.h"

typedef struct _YModemInfo
{
	  int packet_length;
    int file_size;
    uint8_t session_done;
    uint8_t session_begin;
    uint8_t errors;

    uint8_t pakcets_received;
    uint8_t file_done;

    uint8_t finish_check;

    uint8_t packet_data[PACKET_OVERHEAD + PACKET_DATA_LENGTH];
	
	uint8_t revice;
} YModemInfo;

uint8_t FileName[FILE_NAME_LENGTH] = {0};

static int Receive_Byte(uint32_t timeout)
{
    while (timeout-- > 0)
    {
        switch (GetUsartReceiveFinish())
        {
        case 1:
            return 1;
        case 2:
            return 2;
        default:
            delay_1ms(2);
            break;
        }
    }

    return 0;
}

static int Send_Byte(uint8_t c)
{
    SerialPutChar(c);
    return 0;
}

/**
 * @brief
 *
 * @param buffer
 * @param length
 * @param timeout
 * @return int
 *          0 ok
 *          -1 receive timeout or header falied
 *          -2 packet no error
 *          -3 crc error
 */
static int debugLoop=0;
static int Receive_Packet(uint8_t *buffer, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size, computedcrc;
    uint8_t data;
    *length = 0;
	int result=0;
	// receive packet
	usart_start_receive_block((uint32_t)buffer, PACKET_OVERHEAD+PACKET_DATA_LENGTH);
	result = Receive_Byte(timeout);
    if (result == 0)
    {
        return -1;
    }
		//delay_1ms(50);
	data=*buffer;
		//SEGGER_RTT_printf(0,"%d:get Head %X result=%d\r\n",debugLoop++ , data, result);
    switch (data)
    {
    case SOH:
        packet_size = PACKET_SIZE;
        break;
    case STX:
        packet_size = PACKET_DATA_LENGTH;
        break;
    case EOT:
        return 0;
    case CAN:
    //if two CAN return length -1
        if(CAN == *(buffer+1))
        {
            *length=-1;
            return 0;
        }
        else
        {
            return -1;
        }
    default:
        return -3;
    }

    // check packet no
    if (buffer[PACKET_NO_INDEX] != ((buffer[PACKET_NO_N_INDEX] ^ 0xff) & 0xff))
    {
        return -2;
    }

    computedcrc = CalcuCRC(&(buffer[PACKET_HEADER]), (int)packet_size);

    if (computedcrc != (((uint16_t)buffer[packet_size + 3] << 8) | buffer[packet_size + 4]))
    {
        return -3;
    }

    *length = packet_size;

    return 0;
}

/**
 * @brief 完成Ymodem 下Receiver的功能
 *          1.接收数据包 如果是第一次能判定为文件信息描述包
 *          2.接收所有的数据数据包，如果接收到合法的数据包，则提取数据后，直接烧写flash中
 *
 * @param buffer
 * @return int
 */
int Ymodem_Receive(uint8_t *buffer)
{
    YModemInfo yModemInfo;
    int i;
    uint8_t* buf_ptr;
    uint8_t* file_ptr;
    uint8_t fileSizeStr[FILE_SIZE_LENGTH]={0};
		//int fileLength = 0;
    uint32_t flashDestination = APPLICATION_ADDRESS;
    uint32_t ramSourceAddr=0;
		int result=0;

    memset(&yModemInfo, 0, sizeof(YModemInfo));

    //session
    while (yModemInfo.session_done == 0)
    {
        yModemInfo.pakcets_received=0;
        yModemInfo.file_done=0;
        yModemInfo.finish_check=0;
        //packet
        while (yModemInfo.file_done == 0)
        {
						result=Receive_Packet(yModemInfo.packet_data, &(yModemInfo.packet_length), NAK_TIMEOUT);
            switch (result)
            {
            case 0:
                yModemInfo.errors=0;

                switch (yModemInfo.packet_length)
                {
                    //CAN 
                    //abort by Sender
                case -1:
                    Send_Byte(ACK);
                    return 0;
                    //End of this file transmission
                case 0:
                    if(yModemInfo.finish_check==0)
                    {
                        Send_Byte(NAK);
                        yModemInfo.finish_check=1;
                    }
                    else
                    {
                        Send_Byte(ACK);
                        yModemInfo.file_done=1;
                    }
                    break;
                default://Normal Packet
                    //Check receive pakcet number 
                    if((yModemInfo.packet_data[PACKET_NO_INDEX] & 0xff) != (yModemInfo.pakcets_received & 0xff))
                    {
                        Send_Byte(NAK);
                    }
                    else
                    {
                        if(yModemInfo.pakcets_received==0)
                        //Filename Packet
                        {
                            if(yModemInfo.packet_data[PACKET_HEADER] !=0)
                            {
                                for(i=0, file_ptr=yModemInfo.packet_data+PACKET_HEADER;
                                    (*file_ptr!=0) && i<FILE_NAME_LENGTH;)
                                {
                                    FileName[i++]=*file_ptr++;
                                }

                                FileName[i++]='\0';

                                for (i=0, file_ptr++; (*file_ptr != ' ') && (*file_ptr!=0) && (i<FILE_SIZE_LENGTH-1);)
                                {
                                    fileSizeStr[i++]=*file_ptr++;
                                }
                                fileSizeStr[i++]='\0';

                                Str2Int(fileSizeStr, &yModemInfo.file_size);

                                //check the size of image 
                                //if size greater than user can flash size
                                if(yModemInfo.file_size > (int32_t)(USER_FLASH_SIZE))
                                {
                                    //end session
                                    Send_Byte(CAN);
                                    Send_Byte(CAN);

                                    return -1;
                                }

                                Flash_IF_App_Erase(APPLICATION_START_SECTOR, APPLICATION_END_SECTOR);
                                Send_Byte(ACK);
                                Send_Byte(CRC16);
                            }
                            else
                            {
                                Send_Byte(ACK);
                                yModemInfo.file_done=1;
                                yModemInfo.session_done=1;
                                break;
                            }
                        }
                        //Data Packet
                        else
                        {
							// buf_ptr=buffer;
                            // memcpy(buf_ptr, yModemInfo.packet_data+PACKET_HEADER, (uint32_t)yModemInfo.packet_length);

                            // ramSourceAddr=(uint32_t)buf_ptr;
                            ramSourceAddr=(uint32_t)(yModemInfo.packet_data+PACKET_HEADER);
                            if(0 == Flash_IF_Write(&flashDestination, (uint32_t *)ramSourceAddr, yModemInfo.packet_length/4))
                            {
                                Send_Byte(ACK);
                            }
                            else
                            {
                                Send_Byte(CAN);
                                Send_Byte(CAN);
                                return -2;
                            }
                        }

                        yModemInfo.pakcets_received++;
                        yModemInfo.session_begin=1;
                    }
                    break;
                }
                break;
            case 1:
                Send_Byte(CAN);
                Send_Byte(CAN);
                return -3;
            default:
                // 如果session已经开始
                if (yModemInfo.session_begin > 0)
                // 则累计一次错误计数
                {
                    yModemInfo.errors++;
                }

                if (yModemInfo.errors > MAX_ERRORS)
                {
                    Send_Byte(CAN);
                    Send_Byte(CAN);
                    return 0;
                }
                Send_Byte(CRC16); // Send ‘C' to CommInit
                break;
            }
        }
    }

    return yModemInfo.file_size;
}
