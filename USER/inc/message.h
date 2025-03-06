#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "main.h"

#define MESSAGE_RECEIVE_BUFFER_SIZE 28
#define MESSAGE_SEND_BUFFER_SIZE 6

#define CRC16_POLYNOMIAL 0x1021

typedef struct
{
    uint16_t poly;      // 多项式
    uint16_t InitValue; // 初始值
    uint16_t xor ;      // 结果异或值
    uint8_t InputReverse;
    uint8_t OutputReverse;
} CRC_16;

void Message_Receive(void); // 报文接收函数
void Message_Response(uint8_t type, uint8_t result); //报文回复函数,参数为：报文类型，同步结果

uint16_t crc16(uint8_t *addr, int num, CRC_16 type);

uint8_t MeMessage_Analysis(void);

#endif
