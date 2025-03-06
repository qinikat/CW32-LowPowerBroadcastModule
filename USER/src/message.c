#include "../inc/main.h"
#include "message.h"

const CRC_16 crc_16_IBM = {0x8005, 0x0000, 0x0000, TRUE, TRUE};
const CRC_16 crc_16_MAXIM = {0x8005, 0x0000, 0xffff, TRUE, TRUE};
const CRC_16 crc_16_MODBUS = {0x8005, 0xffff, 0x0000, TRUE, TRUE};
// 报文数据缓冲区
uint8_t MessageReceiveBuffer[MESSAGE_RECEIVE_BUFFER_SIZE] = {0x00};

// 报文数据存储区中区
uint8_t Message_Receive_ST[MESSAGE_RECEIVE_BUFFER_SIZE] = {0x00};
uint8_t Message_Send_ST[MESSAGE_SEND_BUFFER_SIZE] = {0x7E, 0x00, 0x00, 0x00, 0x00, 0xE7};
uint8_t Execution_status = 0;

// 报文数据存储区
// 一：报文类型时间同步：系统时间（年-月-日-时-分-秒）共7字节
// 二：报文类型休眠控制：休眠开始时间（年-月-日-时-分）6字节，休眠结束时间（年-月-日-时-分）6字节，系统时间（年-月-日-时-分-秒）7字节
uint8_t Message_Receive1[7] = {0x00};
uint8_t Message_Receive2[19] = {0x00};

uint8_t Flage_Receive_UART3 = 0; // 缓冲区是否有数据
uint8_t Receive_Counted = 0;
uint8_t Receive_Num = 0;

/* 报文状态，状态，
0：等待接收时间同步报文，
1：时间同步后，等待接收休眠控制报文，
2：休眠控制后，等待休眠开始前的时间同步，
*/
uint8_t Message_Status = 0;

extern void delay(uint16_t ms);

uint8_t reverse8(uint8_t data);
uint16_t reverse16(uint16_t data);
uint16_t crc16(uint8_t *addr, int num, CRC_16 type);
uint8_t Data_Normative_Check(uint8_t *data);

uint8_t MeMessage_Analysis_Inner(void);

// 报文接收函数
void Message_Receive(void)
{
    uint8_t i = 0;
    delay(50); // 等待数据接收完毕
    Flage_Receive_UART3 = 0;
    Receive_Num = Receive_Counted;
    Receive_Counted = 0;

    // 将数据发送出去,测试用
    // for (i = 0; i < MESSAGE_RECEIVE_BUFFER_SIZE; i++)
    // {
    //     UART_SendData(CW_UART3, MessageReceiveBuffer[i]);
    //     while (UART_GetFlagStatus(CW_UART3, UART_FLAG_TXE) == RESET)
    //         ;
    // }

    // 打印接收到的数据
    printf("Receive_Num:%d\r\n", Receive_Num);
    for (i = 0; i < Receive_Num; i++)
    {
        printf("%x ", MessageReceiveBuffer[i]);
    }
    printf("\r\n");

    // 将缓冲区数据存储到报文数据存储区中区
    for (i = 0; i < MESSAGE_RECEIVE_BUFFER_SIZE; i++)
    {
        Message_Receive_ST[i] = MessageReceiveBuffer[i];
    }

    // 缓存清零
    for (i = 0; i < MESSAGE_RECEIVE_BUFFER_SIZE; i++)
    {
        MessageReceiveBuffer[i] = 0x00;
    }
}

/*
对接收到的报文进行解析:
一：报文类型时间同步：起始符(1字节)7E,报文类型(1字节)10 为时间同步，系统时间（年-月-日-时-分-秒）共7字节，CRC16校验(2字节),结束符(1字节)E7
二：报文类型休眠控制：起始符(1字节)7E,报文类型(1字节)0A 为休眠控制，休眠开始时间（年-月-日-时）4字节，休眠结束时间（年-月-日-时）4字节，系统时间（年-月-日-时-分-秒）7字节，CRC16校验(2字节),结束符(1字节)E7
*/
uint8_t MeMessage_Analysis(void)
{
    uint8_t Status = 0;
    Status = MeMessage_Analysis_Inner();
    // 将报文数据存储区中区的数据清零
    for (uint8_t i = 0; i < MESSAGE_RECEIVE_BUFFER_SIZE; i++)
    {
        Message_Receive_ST[i] = 0x00;
    }
    return Status;
}

// 报文解析 ,返回值为0，表示报文解析失败，返回值为1，表示报文解析成功且为时间同步，返回值为2，表示报文解析成功且为休眠控制
uint8_t MeMessage_Analysis_Inner(void)
{
    // 判断起始符
    if (Message_Receive_ST[0] != 0x7E)
    {
        return 0;
    }
    // printf("1\r\n");
    // 判断报文类型
    if (Message_Receive_ST[1] == 0x10) // 时间同步
    {
        // 判断报文长度
        if (Receive_Num != 12)
        {
            Message_Response(0x10, 0x00);
            return 0;
        }
        // 判断结束符
        if (Message_Receive_ST[11] != 0xE7)
        {
            Message_Response(0x10, 0x00);
            return 0;
        }
        // printf("2\r\n");

        // 打印crc16校验值
        printf("O-crc16:%x\r\n", ((Message_Receive_ST[9] << 8) | Message_Receive_ST[10]));

        // 打印报文内容
        printf("Message_Receive:");
        for (uint8_t i = 0; i < 7; i++)
        {
            printf("%x ", Message_Receive_ST[i + 2]);
        }

        // 打印计算的crc16校验值
        printf("C-crc16:%x\r\n", crc16(Message_Receive_ST + 2, 7, crc_16_MODBUS));

        // 判断CRC16校验,类型crc_16_IBM
        if (crc16(Message_Receive_ST + 2, 7, crc_16_MODBUS) != ((Message_Receive_ST[9] << 8) | Message_Receive_ST[10]))
        {
            Message_Response(0x10, 0x00);
            return 0;
        }

        // printf("3\r\n");

        // 将报文数据存储区中区的数据存储到报文数据存储区中
        for (uint8_t i = 0; i < 7; i++)
        {
            Message_Receive1[i] = Message_Receive_ST[i + 2];
            printf("%x ", Message_Receive1[i]);
        }

        // printf("4\r\n");

        return 1;
    }

    else if (Message_Receive_ST[1] == 0x0A) // 休眠控制
    {
        // 判断报文长度
        if (Receive_Num != 24)
        {
            Message_Response(0x0A, 0x00);
            return 0;
        }
        // 判断结束符
        if (Message_Receive_ST[23] != 0xE7)
        {
            Message_Response(0x0A, 0x00);
            return 0;
        }

        // printf("6\r\n");

        // 打印crc16校验值
        printf("O-crc16:%x\r\n", ((Message_Receive_ST[21] << 8) | Message_Receive_ST[22]));
        // 打印报文内容
        printf("Message_Receive:");
        for (uint8_t i = 0; i < 19; i++)
        {
            printf("%x ", Message_Receive_ST[i + 2]);
        }
        // 打印计算的crc16校验值
        printf("C-crc16:%x\r\n", crc16(Message_Receive_ST + 2, 19, crc_16_MODBUS));

        // 判断CRC16校验
        if (crc16(Message_Receive_ST + 2, 19, crc_16_MODBUS) != ((Message_Receive_ST[21] << 8) | Message_Receive_ST[22]))
        {
            Message_Response(0x0A, 0x00);
            return 0;
        }
        // printf("7\r\n");

        // 数据规范性校验

        // 将报文数据存储区中区的数据存储到报文数据存储区中
        for (uint8_t i = 0; i < 19; i++)
        {
            Message_Receive2[i] = Message_Receive_ST[i + 2];
        }

        // printf("8\r\n");
        return 2;
    }
    else
    {
        return 0;
    }
}

// 报文测试list
// 时间同步报文： 7E 10  01 01 01 01 01 01 01 BD BC E7
// 休眠控制报文： 7E 0A  01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 5B E7 E7

// 报文回复函数
void Message_Response(uint8_t type, uint8_t result)
{
    // 报文类型为时间同步
    if (type == 0x10)
    {
        // 同步结果为成功
        if (result == 0x01)
        {
            // 回复时间同步成功报文，起始符(1字节)7E,报文类型(1字节)10 为时间同步，同步结果(1字节)01，CRC16校验(2字节),结束符(1字节)E7
            Message_Send_ST[1] = 0x10; // 报文类型
            Message_Send_ST[2] = 0x01; // 同步结果
            Message_Send_ST[3] = 0xC0; // crc16校验高位
            Message_Send_ST[4] = 0xC1; // crc16校验低位
        }
        // 同步结果为失败
        else if (result == 0x00)
        {
            // 回复时间同步失败报文，起始符(1字节)7E,报文类型(1字节)10 为时间同步，同步结果(1字节)00，CRC16校验(2字节),结束符(1字节)E7
            Message_Send_ST[1] = 0x10; // 报文类型
            Message_Send_ST[2] = 0x00; // 同步结果
            Message_Send_ST[3] = 0x00; // crc16校验高位
            Message_Send_ST[4] = 0x00; // crc16校验低位
        }
    }
    // 报文类型为休眠控制
    else if (type == 0x0A)
    {
        // 同步结果为成功
        if (result == 0x01)
        {
            // 回复休眠控制成功报文，起始符(1字节)7E,报文类型(1字节)0A 为休眠控制，同步结果(1字节)01，CRC16校验(2字节),结束符(1字节)E7
            Message_Send_ST[1] = 0x0A; // 报文类型
            Message_Send_ST[2] = 0x01; // 同步结果
            Message_Send_ST[3] = 0xC0; // crc16校验高位
            Message_Send_ST[4] = 0xC1; // crc16校验低位
        }
        // 同步结果为失败
        else if (result == 0x00)
        {
            // 回复休眠控制失败报文，起始符(1字节)7E,报文类型(1字节)0A 为休眠控制，同步结果(1字节)00，CRC16校验(2字节),结束符(1字节)E7
            Message_Send_ST[1] = 0x0A; // 报文类型
            Message_Send_ST[2] = 0x00; // 同步结果
            Message_Send_ST[3] = 0x00; // crc16校验高位
            Message_Send_ST[4] = 0x00; // crc16校验低位
        }
    }
    // 发送报文
    for (uint8_t i = 0; i < MESSAGE_SEND_BUFFER_SIZE; i++)
    {
        UART_SendData(CW_UART3, Message_Send_ST[i]);
        while (UART_GetFlagStatus(CW_UART3, UART_FLAG_TXE) == RESET)
            ;
    }
    // 将报文数据存储区中区的数据清零
    for (uint8_t i = 0; i < MESSAGE_SEND_BUFFER_SIZE - 2; i++)
    {
        Message_Send_ST[i + 1] = 0x00;
    }
}

// 字节反转8位
uint8_t reverse8(uint8_t data)
{
    uint8_t i = 0;
    uint8_t temp = 0;
    for (i = 0; i < 8; i++)
    {
        temp |= ((data >> i) & 0x01) << (7 - i);
    }
    return temp;
}

// 字节反转16位
uint16_t reverse16(uint16_t data)
{
    uint8_t i = 0;
    uint16_t temp = 0;
    for (i = 0; i < 16; i++)
    {
        temp |= ((data >> i) & 0x01) << (15 - i);
    }
    return temp;
}

// 计算CRC16校验值
uint16_t crc16(uint8_t *addr, int num, CRC_16 type)
{
    uint8_t data;
    uint16_t crc = type.InitValue; // 初始值
    int i;
    for (; num > 0; num--)
    {
        data = *addr++;
        if (type.InputReverse == TRUE)
            // 将data字节反转，手动实现
            data = reverse8(data);

        crc = crc ^ (data << 8); // 与crc初始值高8位异或
        for (i = 0; i < 8; i++)  // 循环8位
        {
            if (crc & 0x8000) // 左移移出的位为1，左移后与多项式异或
                crc = (crc << 1) ^ type.poly;
            else // 否则直接左移
                crc <<= 1;
        }
    }
    if (type.OutputReverse == TRUE) // 满足条件，反转
        // 将crc反转，手动实现
        crc = reverse16(crc);
    crc = crc ^ type.xor ; // 最后返与结果异或值异或
    return (crc);          // 返回最终校验值
}
