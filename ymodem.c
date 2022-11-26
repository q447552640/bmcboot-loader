/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-23 23:29:58
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-26 12:03:43
 * @Description: file content
 * @FilePath: \BootLoader\ymodem.c
 */
#include "ymodem.h"
#include "bsp_ocflash.h"
#include "bsp_uart.h"

typedef struct _YModemInfo
{
	  int packet_length;
    int file_size;
    uint8_t session_done;
    uint8_t session_begin;
    uint8_t errors;

    uint8_t pakcets_received;
    uint8_t file_done;

    uint8_t packet_data[PACKET_OVERHEAD + PACKET_DATA_LENGTH];
	
		uint8_t revice[2];
} YModemInfo;

uint8_t FileName[FILE_NAME_LENGTH] = {0};

static int Receive_Byte(uint8_t *c, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (1 == SerialKeyPressed(c))
        {
            return 0;
        }
    }

    return -1;
}

static int Send_Byte(uint8_t c)
{
    SerialPutChar(c);
    return 0;
}

static uint16_t UpdateCrc16(uint16_t crc, uint8_t d)
{
    uint8_t count = 8;
    uint16_t newCrc = crc;
    uint16_t data = d;

    while ((count--) > 0)
    {
        if (newCrc & 0x8000)
        {
            newCrc <<= 1;
            newCrc += (((data <<= 1) & 0x0400) != 0);
            newCrc ^= 0x1021;
        }
        else
        {
            newCrc <<= 1;
            newCrc += (((data <<= 1) & 0x0400) != 0);
        }
    }

    return newCrc;
}

static uint16_t ClcCrc16(const uint8_t *datas, int size)
{
    uint16_t crc = 0;

    const uint8_t *endofDatas = datas + size;

    while (datas < endofDatas)
    {
        crc = UpdateCrc16(crc, *datas);
        datas++;
    }
    crc = UpdateCrc16(crc, 0);
    crc = UpdateCrc16(crc, 0);

    return (crc & (uint16_t)0xffff);
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
static int Receive_Packet(uint8_t *buffer, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size, computedcrc;
    uint8_t data;
    *length = 0;

    if (Receive_Byte(&data, timeout) != 0)
    {
        return -1;
    }

    switch (data)
    {
    case SOH:
        packet_size = PACKET_SIZE;
        break;
    case STX:
        packet_size = PACKET_DATA_LENGTH;
        break;
    case CAN:
    //if two CAN return length -1
        if((Receive_Byte(&data, timeout) == 0) && (CAN == data))
        {
            *length=-1;
            return 0;
        }
        else
        {
            return -1;
        }
    case EOT:
        return 0;
    default:
        return -1;
    }

    *buffer = data; // buffer store header in buffer[0];

    // receive packet
    for (i = 1; i < packet_size; i++)
    {
        if (Receive_Byte(buffer + i, timeout) != 0)
        {
            return -1;
        }
    }

    // check packet no
    if (buffer[PACKET_NO_INDEX] != ((buffer[PACKET_NO_N_INDEX] ^ 0xff) & 0xff))
    {
        return -2;
    }

    computedcrc = ClcCrc16(buffer[PACKET_HEADER], (int)packet_size);

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

    memset(&yModemInfo, 0, sizeof(YModemInfo));



    //session
    while (yModemInfo.session_done == 0)
    {
        yModemInfo.pakcets_received=0;
        yModemInfo.file_done=0;
        //packet
        while (yModemInfo.file_done == 0)
        {
            switch (Receive_Packet(yModemInfo.packet_data, &(yModemInfo.packet_length), NAK_TIMEOUT))
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
                    Send_Byte(ACK);
                    yModemInfo.file_done=1;
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
                            if(0 == Flash_IF_Write(&flashDestination, (uint32_t *)ramSourceAddr, yModemInfo.pakcets_received/4))
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
