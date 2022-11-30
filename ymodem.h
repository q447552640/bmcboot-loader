/*
 * @Author: Ma Yuchen
 * @Date: 2022-11-23 23:30:07
 * @LastEditors: Ma YuChen
 * @LastEditTime: 2022-11-24 18:51:22
 * @Description: Y Modem File Transport Protocol Define func
 *                  this Protocel can be used when uart is only IO interface on MCU
 * @FilePath: \BootLoader\ymodem.h
 */
#ifndef Y_MODEM_H
#define Y_MODEM_H
#include <stdint.h>
#include <string.h>

/*Ymodem Frame base on ASCII Encode*/
#define SOH (0x01)
#define STX (0x02)
#define ETX (0x03)
#define EOT (0x04)
#define ENQ (0x05)
#define ACK (0x06)
#define NAK (0x15)
#define CAN (0x18)
#define CRC16 (0x43) // YModem C on hex = 0x43

#define PACKET_HEADER       (3)
#define PACKET_TRAILER      (2)
#define PACKET_OVERHEAD     (PACKET_HEADER+PACKET_TRAILER)
#define PACKET_SIZE         (128)
#define PACKET_DATA_LENGTH (1024)

#define FILE_NAME_LENGTH    (256)
#define FILE_SIZE_LENGTH    (16)

#define PACKET_NO_INDEX     (1)
#define PACKET_NO_N_INDEX   (2)


//#define NAK_TIMEOUT         (0x100000)
#define NAK_TIMEOUT         (500)
#define MAX_ERRORS          (5)

extern uint8_t FileName[];

int Ymodem_Receive(uint8_t* buffer);

#endif
