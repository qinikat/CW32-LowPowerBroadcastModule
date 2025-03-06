/*******************************************************************************
 *
 * 代码许可和免责信息
 * 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
 * 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
 * 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
 * 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
 * 或条件。
 * 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
 * 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
 * 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
 * 损失。
 * 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
 * 全部上述排除或限制可能并不适用于您。
 *
 *******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "../inc/main.h"

#include "rtc.h"
#include "i2c.h"
#include "message.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void BTIM_Configuration(void);
void delay(uint16_t ms);

void DeepSleep_GOto(void);
void UART3_Init(uint32_t BaudRate);

extern uint8_t Sleep_Status;
extern uint8_t Flage_Receive_UART3;
extern uint8_t Message_Status;
extern uint8_t Message_Receive1[7];
extern uint8_t Message_Receive2[19];

static uint8_t const pow2_table[] = {0, 1, 2, 3, 4, 5, 6, 7};

// 初始化RTC，设置时间为2023年6月21日6点58分59秒
uint8_t init_RTC_Time[6] = {0x23, 0x06, 0x21, 0x06, 0x58, 0x59};

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample toggle GPIOA.00
 **
 ******************************************************************************/

/**
 * @brief  RTC时钟测试，初始化日历，使用间隔中断0.5秒通过Log输出日期时间
 * @return int32_t
 */

int32_t main(void)
{
    /* System Clocks Configuration */
    // 初始化内部时钟源,内部8M,外部32.768K,以及APB.
    RCC_Configuration();

    // 基本定时器初始化
    BTIM_Configuration();

    /* GPIO Configuration */
    // 设置控制 PA00作为 724 的电源控制引脚
    GPIO_Configuration();

    /* NVIC Configuration */
    /* 设置终端向量表 */
    NVIC_Configuration();

    /* 串口日志UART4初始化 */
    LogInit();

    /* 与724串口通信,UART3初始化 */
    UART3_Init(115200);

    // 已改为串口通信，i2c驱动已舍弃
    //  i2c1_init();

    // 初始化RTC，设置时间为2023年6月21日6点58分59秒
    RTC_datetime_set(init_RTC_Time);

    // RTC_AlarmA_Set(0x07,0x00);
    // RTC_Sleep_Set(0x23, 0x06, 0x21, 0x08, 0x57, 0x23, 0x06, 0x21, 0x08, 0x01);
    // 打印重启信息
    printf("=====Restart...\r\n");

    while (1)
    {

        // 判断sleep_status状态
        if (Sleep_Status == 0 || Sleep_Status == 1)
        {
            // 拉高PA00，724上电
            PA00_SETHIGH();

            if (Flage_Receive_UART3 == 1) // 等待串口接收到数据
            {
                Message_Receive();                     // 接收报文
                Message_Status = MeMessage_Analysis(); // 解析报文,并更新状态

                if (Message_Status == 1)
                {
                    // 存入Syn_Time,用于时间同步
                    uint8_t Syn_Time[6] = {0};
                    for (uint8_t i = 0; i < 6; i++)
                    {
                        Syn_Time[i] = Message_Receive1[i + 1];
                    }

                    // 判断时间的合法性
                    if (RTC_Time_Check(Syn_Time, 6) == 0)
                    {
                        // 回复错误报文
                        Message_Response(0x10, 0x00);
                        printf("Receive time synchronization mes sage failed\r\n");
                    }
                    else
                    {
                        // 进行时间同步
                        RTC_datetime_set(Syn_Time);
                        printf("RTC_datetime_set:%x-%x-%x %x:%x:%x\r\n", Syn_Time[0], Syn_Time[1], Syn_Time[2], Syn_Time[3], Syn_Time[4], Syn_Time[5]);

                        // 回复报文,报文类型为时间同步,报文结果为成功
                        Message_Response(0x10, 0x01);
                        printf("Receive time synchronization message successfully\r\n");
                    }
                    // 更新状态，等待下一次接收
                    Message_Status = 0;
                }

                else if (Message_Status == 2)
                {

                    uint8_t Syn_Time[6] = {0};
                    uint8_t data_sleep_start[5] = {0};
                    uint8_t data_sleep_end[5] = {0};

                    // 存入Syn_Time,用于时间同步
                    for (uint8_t i = 0; i < 6; i++)
                    {
                        Syn_Time[i] = Message_Receive2[i + 13];
                    }
                    // 存入data_sleep_start与data_sleep_end,用于设置休眠起始闹钟时间
                    for (uint8_t i = 0; i < 5; i++)
                    {
                        data_sleep_start[i] = Message_Receive2[i + 1];
                        data_sleep_end[i] = Message_Receive2[i + 7];
                    }

                    // 判断时间的合法性
                    if (RTC_Time_Check(Syn_Time, 6) == 0 || RTC_Time_Check(data_sleep_start, 5) == 0 || RTC_Time_Check(data_sleep_end, 5) == 0 || RTC_Time_Compare(data_sleep_start, data_sleep_end, 0) == 0 || RTC_Time_Compare(Syn_Time, data_sleep_start, 1) == 0)
                    {
                        // 分别打印4个判断的合法性
                        printf("RTC_Time_Check(Syn_Time, 6) = %d\r\n", RTC_Time_Check(Syn_Time, 6));
                        printf("RTC_Time_Check(data_sleep_start, 5) = %d\r\n", RTC_Time_Check(data_sleep_start, 5));
                        printf("RTC_Time_Check(data_sleep_end, 5) = %d\r\n", RTC_Time_Check(data_sleep_end, 5));
                        printf("RTC_Time_Compare(data_sleep_end, data_sleep_start) = %d\r\n", RTC_Time_Compare(data_sleep_start, data_sleep_end, 0));
                        printf("RTC_Time_Compare(Syn_Time, data_sleep_start) = %d\r\n", RTC_Time_Compare(Syn_Time, data_sleep_start, 1));
                        // 回复错误报文
                        Message_Response(0x0A, 0x00);
                        printf("Receive sleep control message failed\r\n");
                        printf("RTC_Time_Check failed\r\n");
                    }
                    else
                    {
                        // 进行时间同步
                        RTC_datetime_set(Syn_Time);
                        // 打印时间同步信息
                        printf("RTC_datetime_set:%x-%x-%x %x:%x:%x\r\n", Syn_Time[0], Syn_Time[1], Syn_Time[2], Syn_Time[3], Syn_Time[4], Syn_Time[5]);
                        // 设置休眠起始闹钟时间
                        RTC_Sleep_Set(data_sleep_start, data_sleep_end);
                        // 回复成功报文
                        Message_Response(0x0A, 0x01);
                        // 更新状态
                        Sleep_Status = 1;
                        printf("Receive sleep control message successfully\r\n");
                    }
                    // 更新状态，等待下一次接收
                    Message_Status = 0;
                }

                else if (Message_Status == 0) // 报文解析失败，状态不变等待下一次接收
                {
                    Message_Status = 0;
                    printf("Receive sleep control message failed\r\n");
                }
            }
        }

        // else if (Sleep_Status == 1)
        // {
        // }
        else if (Sleep_Status == 2)
        {
            // 休眠模式
            printf("=====Enter DeepSleep Mode...\r\n");
            PA00_SETLOW();
            DeepSleep_GOto();
        }
        else
        {
            // 程序不应该运行到这里
            printf("Sleep_Status error\r\n");
        }
    }
}

// 数据测试
/*
7E 10 20 23 06 21 07 20 20 74 16 E7
7E 0A   20 23 06 21 07 22   20 23 06 21 07 23   20 23 06 21 07 21   20   81D6  E7  1分钟休眠测试
7E 0A   20 23 06 21 07 22   20 23 06 21 08 24   20 23 06 21 07 20   21   C716  E7   1小时2分钟休眠测试
7E 0A   20 23 06 21 07 24   20 23 06 21 13 19   20 23 06 21 07 20   21   BC6C  E7   5小时58分钟休眠测试
7E 0A   20 23 09 22 02 44   20 23 06 21 08 24   20 23 06 21 02 45   10   D60F   E7

//错误数据
7E 0A   20 23 09 22 02 44   20 23 06 21 08 24   20 23 06 21 02 45 10   253A   E7
*/

/*
 * @brief  Configures the different system clocks.
 * @param  None
 * @retval None
 */

void BTIM_Configuration(void)
{
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct = {0};
    __RCC_BTIM_CLK_ENABLE();

    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER;
    BTIM_TimeBaseInitStruct.BTIM_Period = 49999;
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = 15; // 16分频

    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);
}

void RCC_Configuration(void)
{
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);
    // RCC_LSI_Enable();
    RCC_LSE_Enable(RCC_LSE_MODE_OSC, RCC_LSE_AMP_LARGER, RCC_LSE_DRIVER_LARGER);

    // RCC_HSE_Enable(RCC_HSE_MODE_OSC, 8000000, RCC_HSE_DRIVER_SMALL, RCC_HSE_FLT_CLOSE);

    RCC_APBPeriphClk_Enable1(RCC_APB1_PERIPH_RTC, ENABLE);

    // 使能I2C1时钟
    CW_SYSCTRL->APBEN1_f.I2C1 = 1U;
}

/**
 * @brief  Configure the GPIO Pins.
 * @param  None
 * @retval None
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    CW_SYSCTRL->AHBEN_f.GPIOA = 1; // Open GPIOA Clk
    // set PA00  as output
    GPIO_InitStruct.Pins = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);

    // 默认PA00上电输出高电平
    PA00_SETHIGH();
}

/**
 * @brief  Configure the nested vectored interrupt controller.
 * @param  None
 * @retval None
 */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(BTIM1_IRQn);
    NVIC_SetPriority(RTC_IRQn, 0);
    NVIC_EnableIRQ(RTC_IRQn);
    // NVIC_EnableIRQ(I2C1_IRQn);
    NVIC_SetPriority(UART3_UART6_IRQn, 0);
    NVIC_EnableIRQ(UART3_UART6_IRQn);
    __enable_irq();
}

void delay(uint16_t ms)
{
    uint32_t i;
    for (; ms > 0; ms--)
    {
        i = 796;
        while (i--)
            ;
    }
}

void DeepSleep_GOto(void)
{
    PWR_InitTypeDef PWR_InitStructure = {0};

    /* Enter Stop Mode */
    PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
    PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Enable;
    PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
    PWR_Config(&PWR_InitStructure);

    PWR_GotoLpmMode();
}

void UART3_Init(uint32_t BaudRate)
{
    uint32_t PCLK_Freq;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    UART_InitTypeDef UART_InitStructure = {0};

    PCLK_Freq = SystemCoreClock >> pow2_table[CW_SYSCTRL->CR0_f.HCLKPRS];
    PCLK_Freq >>= pow2_table[CW_SYSCTRL->CR0_f.PCLKPRS];

    // 724通信串口使用UART3
    //  PA02->TX
    //  PA03<-RX
    // 时钟使能
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_UART3_CLK_ENABLE();

    // 先设置UART TX RX 复用，后设置GPIO的属性，避免口线上出现毛刺
    // PB08_AFx_UART5TXD();
    // PB09_AFx_UART5RXD();
    PA02_AFx_UART3TXD();
    PA03_AFx_UART3RXD();

    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_3;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    // GPIO_InitStructure.Pins = GPIO_PIN_8;
    // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    // GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    // GPIO_InitStructure.Pins = GPIO_PIN_9;
    // GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    // GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    UART_InitStructure.UART_BaudRate = BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = PCLK_Freq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    // UART_Init(CW_UART5, &UART_InitStructure);
    UART_Init(CW_UART3, &UART_InitStructure);

    // 使能UARTx RC中断
    UART_ITConfig(CW_UART3, UART_IT_RC, ENABLE);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

void run1(void)
{ // Sleep_Status =0,为正常工作模式
    if (Sleep_Status == 0)
    {
        // 拉高PA00，724上电
        PA00_SETHIGH();
        // 状态0, 等待接收时间同步报文
        if (Message_Status == 0)
        {
            if (Flage_Receive_UART3 == 1) // 等待接收到数据
            {
                Message_Receive();                     // 接收报文
                Message_Status = MeMessage_Analysis(); // 解析报文,并更新状态

                if (Message_Status == 1)
                {
                    // 存入Syn_Time,用于时间同步
                    uint8_t Syn_Time[6] = {0};
                    for (uint8_t i = 0; i < 6; i++)
                    {
                        Syn_Time[i] = Message_Receive1[i + 1];
                    }

                    // 判断时间的合法性
                    if (RTC_Time_Check(Syn_Time, 6) == 0)
                    {
                        // 回复错误报文
                        Message_Response(0x10, 0x00);
                        Message_Status = 0;
                        printf("Receive time synchronization message failed\r\n");
                    }
                    else
                    {

                        // 进行时间同步
                        RTC_datetime_set(Syn_Time);
                        printf("RTC_datetime_set:%x-%x-%x %x:%x:%x\r\n", Syn_Time[0], Syn_Time[1], Syn_Time[2], Syn_Time[3], Syn_Time[4], Syn_Time[5]);

                        // 回复报文,报文类型为时间同步,报文结果为成功
                        Message_Response(0x10, 0x01);
                        printf("Receive time synchronization message successfully\r\n");
                    }
                }

                else if (Message_Status == 2)
                {

                    // 回复错误报文
                    Message_Response(0x0A, 0x00);
                    Message_Status = 0;
                    printf("Receive sleep control message failed\r\n");
                }
            }
        }

        // 状态1,等待接收休眠控制报文
        else if (Message_Status == 1)
        {
            if (Flage_Receive_UART3 == 1) // 等待接收到数据
            {
                Message_Receive();                     // 接收报文
                Message_Status = MeMessage_Analysis(); // 解析报文,并更新状态

                if (Message_Status == 2)
                {
                    uint8_t Syn_Time[6] = {0};
                    uint8_t data_sleep_start[5] = {0};
                    uint8_t data_sleep_end[5] = {0};

                    // 存入Syn_Time,用于时间同步
                    for (uint8_t i = 0; i < 6; i++)
                    {
                        Syn_Time[i] = Message_Receive2[i + 13];
                    }
                    // 存入data_sleep_start与data_sleep_end,用于设置休眠起始闹钟时间
                    for (uint8_t i = 0; i < 5; i++)
                    {
                        data_sleep_start[i] = Message_Receive2[i + 1];
                        data_sleep_end[i] = Message_Receive2[i + 7];
                    }

                    // 判断时间的合法性
                    if (RTC_Time_Check(Syn_Time, 6) == 0 || RTC_Time_Check(data_sleep_start, 5) == 0 || RTC_Time_Check(data_sleep_end, 5) == 0 || RTC_Time_Compare(data_sleep_start, data_sleep_end, 0) == 0 || RTC_Time_Compare(Syn_Time, data_sleep_start, 1) == 0)
                    {
                        // 分别打印4个判断的合法性
                        printf("RTC_Time_Check(Syn_Time, 6) = %d\r\n", RTC_Time_Check(Syn_Time, 6));
                        printf("RTC_Time_Check(data_sleep_start, 5) = %d\r\n", RTC_Time_Check(data_sleep_start, 5));
                        printf("RTC_Time_Check(data_sleep_end, 5) = %d\r\n", RTC_Time_Check(data_sleep_end, 5));
                        printf("RTC_Time_Compare(data_sleep_end, data_sleep_start) = %d\r\n", RTC_Time_Compare(data_sleep_start, data_sleep_end, 0));
                        printf("RTC_Time_Compare(Syn_Time, data_sleep_start) = %d\r\n", RTC_Time_Compare(Syn_Time, data_sleep_start, 1));
                        // 回复错误报文
                        Message_Response(0x0A, 0x00);
                        Message_Status = 1;
                        printf("Receive sleep control message failed\r\n");
                        printf("RTC_Time_Check failed\r\n");
                    }
                    else
                    {
                        // 进行时间同步
                        RTC_datetime_set(Syn_Time);

                        // 打印时间同步信息
                        printf("RTC_datetime_set:%x-%x-%x %x:%x:%x\r\n", Syn_Time[0], Syn_Time[1], Syn_Time[2], Syn_Time[3], Syn_Time[4], Syn_Time[5]);

                        // 设置休眠起始闹钟时间
                        RTC_Sleep_Set(data_sleep_start, data_sleep_end);
                        // 回复成功报文
                        Message_Response(0x0A, 0x01);
                        // 更新状态
                        Message_Status = 2;
                        Sleep_Status = 1;
                        printf("Receive sleep control message successfully\r\n");
                    }
                }

                else if (Message_Status == 1)
                {
                    // 回复错误报文
                    Message_Response(0x10, 0x00);
                    printf("Receive time synchronization message failed\r\n");
                }

                else if (Message_Status == 0) // 报文解析失败，状态不变等待下一次接收
                {
                    Message_Status = 1;
                    printf("Receive sleep control message failed\r\n");
                }
            }
        }
    }
    else if (Sleep_Status == 1) // 等待休眠开始，并在休眠开始前进行时间同步
    {
    }
    // 根据Sleep_Status判断是否进入休眠模式
    else if (Sleep_Status == 2)
    {
        PA00_SETLOW();
        printf("=====Enter DeepSleep Mode...\r\n");
        DeepSleep_GOto();
    }
    else
    {
        // 程序不应该运行到这里
        printf("Sleep_Status error\r\n");
    }
}

// 测试报文通信
// if (TRUE)
// {
//     if (Flage_Receive_UART3 == 1) // 等待接收到数据
//     {
//         Message_Receive();                     // 接收报文
//         Message_Status = MeMessage_Analysis(); // 解析报文,并更新状态
//         if (Message_Status == 1)
//         {
//             // 打印成功接收到时间同步报文
//             printf("Receive time synchronization message successfully\r\n");
//             Message_Response(0x10, 0x01); // 回复报文,报文类型为时间同步,报文结果为成功
//         }

//         if (Message_Status == 2)
//         {
//             // 打印成功接收到休眠控制报文
//             printf("Receive sleep control message successfully\r\n");
//             Message_Response(0x0A, 0x01);
//         }
//     }
// }
